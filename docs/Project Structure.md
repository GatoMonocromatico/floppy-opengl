---
tags: [moc, tetris-roguelike]
---

# Project Structure

Map of content for the Tetris Roguelike codebase. Start here, then follow the links
below into whichever layer you need.

```
Tetris_Roguelike/
├── src/
│   ├── main.cpp        entry point: window/SDL setup, main loop
│   ├── glad.c           generated OpenGL function loader (glad)
│   ├── render/          → [[Render]]
│   ├── game/            → [[Game]]
│   └── util/            → [[Util]]
├── assets/              → [[Assets]]
│   ├── shaders/
│   └── textures/
├── Libraries/            third-party SDKs (SDL3, SDL3_image, glad, glm) — vendored, not project code
├── build/                CMake build output, git-ignored, safe to delete
├── docs/                 you are here
└── CMakeLists.txt, run.sh → [[Build System]]
```

## Layering

`main.cpp` is the only file that reaches into all three `src/` layers. Dependencies
flow one way:

```
game/  →  render/  →  util/
```

- [[Game]] (`src/game/`) is the Tetris-roguelike logic: grid state, bricks, AI, and the
  `Resources` bundle that ties render objects to game state.
- [[Render]] (`src/render/`) is a thin OpenGL wrapper layer with no knowledge of game
  rules — buffers, shaders, textures, meshes, camera.
- [[Util]] (`src/util/`) has no project dependencies at all — logging, timing, math
  helpers used by both other layers.

`game/` code includes from `render/` and `util/`; `render/` code includes from `util/`
only; nothing in `render/` or `util/` includes from `game/`.

## Include convention

Every `#include` is written relative to `src/` (which is the sole include root added in
`CMakeLists.txt`), including same-folder includes, e.g. inside `src/render/camera.h`:

```cpp
#include "render/shaderClass.h"
#include "util/DebugLog.h"
```

This makes it obvious at a glance which layer a dependency comes from, without needing
to know the including file's own location.

## See also
- [[Assets]] — how shaders/textures are loaded at runtime
- [[Build System]] — CMakeLists.txt, Libraries/, run.sh
- [[Entity Handles]] — how game entities are stored and addressed
- [[Memory Safety]] — debugging heap corruption in this codebase
- [[AI]] and [[Attack Gauge]] — the enemy player and the mechanic it plays
