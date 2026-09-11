---
tags: [tetris-roguelike, build]
---

# Build System

## CMakeLists.txt

Cross-platform build (Linux + Windows) via CMake ≥ 3.20, C++20.

- `SRC_DIR` (`${PROJECT_ROOT}/src`) is the sole include root for project headers — every
  `#include` in the codebase is written relative to it (`render/...`, `game/...`,
  `util/...`), see [[Project Structure#Include convention]].
- `LOCAL_INCLUDE_DIR`/`LOCAL_LIB_DIR` point into `Libraries/` for the vendored SDK
  headers and platform-specific prebuilt libraries (`Libraries/lib_win/lib` on Windows,
  `Libraries/lib_linux/lib` on Linux) — configure fails fast with a clear error if
  either is missing.
- The executable target lists every `.cpp`/`.c` under `src/` explicitly (no globbing),
  plus `src/glad.c`.
- `DEBUG` is defined only for Debug builds, gating the `DBG`/`MDBG` macros in
  [[Util#DebugLog|DebugLog.h]].
- Linux: links `GL dl m`, sets `RPATH` so the binary finds `Libraries/lib_linux/lib`'s
  shared objects without needing `LD_LIBRARY_PATH`.
- Windows: links `opengl32`, copies `SDL3.dll`/`SDL3_image.dll` next to the built exe as
  a post-build step.

## Libraries/

Vendored third-party code and prebuilt binaries — not project source, never edited:

```
Libraries/
├── include/       SDL3, SDL3_image, glad, glm headers
├── lib_linux/lib/  prebuilt SDL3 .so files for Linux
└── lib_win/lib/    prebuilt SDL3 .lib/.dll files for Windows
```

## run.sh

Convenience script: configures + builds a Debug build, then runs the executable under
`gdb -batch -ex run -ex "bt full"` for an automatic crash backtrace. Always `cd`s to the
project root first — this is what makes the relative asset paths in [[Assets]] resolve
correctly.

## Debug hooks

Two environment variables, both no-ops when unset, both added to make a 3D effect
verifiable without a human at the keyboard (see [[Portal]]):

- `TR_CAMERA="x,y,z[,yaw,pitch]"` — repositions the camera at startup. An effect that
  is only ever checked head-on has not been checked.
- `TR_PORTAL="x,y,z[,yaw,pitch,scale]"` — repositions and resizes the [[Portal]] at
  startup. Sizing it is a visual judgement, so this makes trying a size a rerun
  rather than a rebuild.
- `TR_SHOT=<path>` (with optional `TR_SHOT_AFTER=<seconds>`, default 2) — renders for
  that long, writes one PNG and exits. Captures the back buffer directly, so it is
  exactly one rendered frame with no compositor or window focus involved.

In-game: `F12` writes `screenshot.png`, `M` replays the portal's materialisation.

## See also
- [[Project Structure]] — where everything CMake compiles actually lives
- [[Assets]] — why the working directory matters at runtime
