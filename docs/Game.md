---
tags: [tetris-roguelike, game]
---

# Game (`src/game/`)

Tetris-roguelike game logic: grid/brick rules, AI, and the resource bundle that ties
[[Render]] objects to game state. Depends on both [[Render]] and [[Util]].

## Core loop & state

- **gameCore** (`gameCore.h/.cpp`) — per-frame grid update logic (e.g. clearing full
  lines); pulls together `GameState`, `gameObject`, and `brickUtils`.
- **GameState** (`GameState.h/.cpp`) — top-level mutable game state: key bindings
  (`ConfiguratedKeys`), timers, and owns the `Resources` and `AI` instances for a run.
- **gameObject** (`gameObject.h/.cpp`) — grid/brick data types (including
  `RowStatistics`) and the game-object entities placed on the grid; the biggest header
  in this layer, includes from [[Render]] (`camera`, `animation`, `shaderClass`) and
  [[Game]] (`Resources`).

## Bricks & AI

- **brickUtils** (`brickUtils.h/.cpp`) — brick shape/placement helpers operating on
  `gameObject` types.
- **AI** (`AI.h/.cpp`) — opponent/assist AI; depends on `gameObject` and `Timer`
  ([[Util]]).

## Resources

- **Resources** (`Resources.h/.cpp`) — the bundle of everything the renderer needs for
  a frame: `Camera`, lighting UBOs, `Texture`s, `Animation`s, `Mesh`es, `Point`s, and
  the compiled `Shader` programs. `Resources::load()` is where the texture paths under
  `assets/textures/` (see [[Assets]]) get wired up. This is the seam between [[Game]]
  and [[Render]] — it's a game-level struct, but every member type it holds comes from
  `render/`.

## Dependency shape

```
GameState ──> gameObject, Resources, AI, Timer(util)
gameCore  ──> GameState, gameObject, brickUtils
AI        ──> gameObject, GameState, gameCore, brickUtils, Timer(util)
Resources ──> Mesh, Light, Point, Animation, Texture (all render/), mathUtils(util)
```

## See also
- [[Render]] — the GPU-facing types `Resources` assembles
- [[Util]] — `Timer`, `DebugLog`, `mathUtils` used throughout this layer
- [[Project Structure]] — overall layering rules
