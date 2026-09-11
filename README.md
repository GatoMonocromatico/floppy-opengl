# Tetris Roguelike (floppy-opengl)

A 3D competitive Tetris built on a from-scratch C++/OpenGL engine: two players each play their own game of Tetris, but every line you clear attacks your opponent's board through a magical portal that renders their game live, in-scene, in real time.

Everything is built around one rule: the player is a person standing in a room, not a player at a menu. Every mechanic has to make sense from inside that fiction (see `docs/World and Lore.md`) — which is why the portal isn't a HUD overlay, it's a rendered object in the world.

## What it does

- **Portal rendering** (`src/render/Portal.cpp`, `Framebuffer.cpp`): the opponent's board is rendered head-on to an offscreen framebuffer every frame, then painted onto the portal's mesh in the scene. It's deliberately not true perspective/parallax, so it stays readable from anywhere in the room rather than only from directly in front of it.
- **AI opponent** (`src/game/AI.cpp`): a bitboard beam search over hypothetical boards, tracking running board statistics instead of recomputing full board state per candidate move.
- **Enforced layered architecture**: `game/` → `render/` → `util/`, one direction only — `render/` has no knowledge of game rules, `util/` depends on nothing project-specific. Documented in `docs/Project Structure.md` and kept in sync with the code as a standing rule (`CLAUDE.md`).
- **Living documentation**: `docs/` is an Obsidian vault (`World and Lore`, `Portal`, `AI`, `Memory Safety`, `Entity Handles`, `Build System`, ...), not a one-time write-up.

## Build

Cross-platform CMake + SDL3 (Linux and Windows, vendored dependencies under `Libraries/` — no system-wide installs needed):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/tetris_roguelike
```

`./run.sh` does the same and runs under `gdb` for a crash backtrace. A `build-asan/` variant exists for memory-safety debugging — `docs/Memory Safety.md` documents a real heap-corruption bug that was caught and fixed this way.

## Status

An early, playable slice: competition today is player vs. offline AI, single cabinet each. Built on top of [`opengl-quickstart`](https://github.com/GatoMonocromatico/opengl-quickstart), the from-scratch engine base this project extends.
