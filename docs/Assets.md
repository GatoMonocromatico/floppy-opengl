---
tags: [tetris-roguelike, assets]
---

# Assets (`assets/`)

Runtime-loaded files — not compiled, just read off disk when the game starts.

```
assets/
├── shaders/    default.vert, default.frag, point.vert, point.frag, brickUnit.frag
└── textures/   arcade.png, brick-unit-skins.png, enemy-portal-spawn-anim.png, ...
```

## How paths resolve

Both `Shader` and `Texture` ([[Render]]) load files by a plain relative path — there is
no base-directory prefixing in the loading code itself (`shaderClass.cpp` opens the
path via `std::ifstream` as given; `texture.cpp` passes it straight to SDL_image). The
path is only ever correct if the process's **working directory is the project root**,
which is what `run.sh` guarantees by `cd`-ing there before building/running (see
[[Build System]]).

Concretely:
- `src/main.cpp` constructs shaders as `Shader("assets/shaders/default.vert",
  "assets/shaders/default.frag")`, etc.
- `src/game/Resources.cpp` (`Resources::load()`) constructs textures as
  `Texture("assets/textures/arcade.png", ...)`, etc.

If you ever run the built binary directly from `build/` instead of via `run.sh`, these
loads will fail — always run from the project root, or `cd` there first.

## Tracking

`assets/textures/` is listed in `.gitignore` (`/assets/textures`) — texture binaries
aren't committed to the repo. `assets/shaders/` is tracked normally (they're small text
files and part of the reviewable diff).

## See also
- [[Render]] — the `Shader`/`Texture` classes that load these files
- [[Build System]] — `run.sh`, which sets the working directory these relative paths
  depend on
