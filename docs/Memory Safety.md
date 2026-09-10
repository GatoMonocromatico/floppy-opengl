---
tags: [tetris-roguelike, debugging, game]
---

# Memory Safety

Findings from the 2026-09-08 investigation into
`malloc_consolidate(): unaligned fastbin chunk` crashing after `SDL_GL_SwapWindow`.

## The symptom lies about the cause

The crash surfaces inside the allocator during drawing, but nothing in [[Render]] is
at fault. Heap metadata is corrupted earlier, in [[Game]] logic; the mesh/instance
allocations in `drawGrid` are simply the next big `malloc`/`free` to walk the damaged
free list, so that is where glibc aborts. Commenting out draw calls only moves the
abort — it does not remove the corruption.

**Consequence:** never bisect this class of bug by commenting out draw calls. Use
AddressSanitizer, which reports the bad access itself rather than the later victim.

## Reproducing

```bash
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g" \
  -DCMAKE_C_FLAGS="-fsanitize=address -g" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address"
cmake --build build-asan -j8
ASAN_OPTIONS=detect_leaks=0 ./build-asan/tetris_roguelike
```

Enabling `AIUpdate` for grid 1 turns the AI into a stress harness: it hard-drops a
piece roughly twice a second, so line clears and `updateGrid` compaction are exercised
in seconds instead of by hand.

## The systemic pattern: references into growing vectors

All confirmed bugs are the same mistake. A `T&` (or `stdMat` row reference) is bound
into a `std::vector`, then that vector is grown — `push_back` reallocates, the old
buffer is freed, and the reference dangles. The next write through it corrupts a freed
heap chunk.

The three vectors this happens to are `GridData::currentUnits`,
`GridData::currentBricks`, and `BrickData::units`.

### 1. `createPlayingBrick` — preview unit reference

`gameCore.cpp:9` binds `previewUData` into `currentUnits`. `createUnit` then
`push_back`s four times into that same vector (`gameCore.cpp:28`). The writes at
`gameCore.cpp:30` and `:44` go through the dangling reference.

Fires on the **first frame**, before any piece has ever locked. Fix: hold the
`size_t` index, re-index `currentUnits` at each use.

### 2. `main.cpp` — playing-brick reference across a lock-in

`main.cpp:286` binds `playingBrick` to `currentBricks.back()`. The lock-in path calls
`playingPieceDropped` (`main.cpp:342`), which spawns the next piece and `push_back`s
into `currentBricks`. `main.cpp:344` then writes through the dangling reference.

That line is also a logic bug in its own right: it reads as "re-point at the new
brick", but a C++ reference cannot rebind, so it *copy-assigns* the new brick over the
old one. It needs to be a pointer.

This is the one that fires on hard drop.

### 3. `cleanFullLines` — stale `GridCell::unitNum`

`gameCore.cpp:519` does `currentBricks[cell.brickIndex].units[cell.unitNum]`. Both
indices are stale: `updateGrid`'s deprecated-unit compaction shrinks
`BrickData::units` with `erase` and renumbers survivors, but grid cells still hold the
pre-compaction `unitNum`. Reported by ASan as a heap-buffer-overflow read.

The compaction that creates the staleness is itself unsound. `updateGrid:690`
recomputes `specificDataLocation` by subtracting an accumulator, but
`acumulatedDeltaFromPrevBricks` accumulates the count of *kept* units rather than the
count *removed* (`updateGrid:698`), so the arithmetic drifts. `currentUnits` is never
actually shrunk to match either — measured growth is +4 units per piece with no
reclamation, even across line clears.

## Root cause under the pattern

The `currentBricks` / `currentUnits` / `specificDataLocation` /
`indexInCurrentBricks` / `unitNum` web is five cross-indices into three vectors that
all resize independently, with no single owner keeping them consistent. Every one of
the bugs above is a consequence of that shape, not an isolated slip.

A locked Tetris cell needs no object identity — only its `Piece` (for the texture
step). `drawGrid` already reads the board cell-by-cell and only needs `uData.shape`
plus "is this the falling piece". So the indirection is load-bearing for nothing, and
collapsing it to a plain `Piece` grid plus one active-piece struct would delete all
five indices and all three bug classes together.

## See also
- [[Game]] — the layer all of this lives in
- [[AI]] — why the incremental statistics these bugs live in are not needed
- [[Build System]] — the CMake setup the ASan build overrides
