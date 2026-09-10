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
- **gameObject** (`gameObject.h/.cpp`) — grid/brick data types and the game-object
  entities placed on the grid; the biggest header in this layer, includes from
  [[Render]] (`camera`, `animation`, `shaderClass`) and [[Game]] (`Resources`). Holds
  the `currentBricks` / `currentUnits` pools and their free lists — read
  [[Entity Handles]] before touching them, and `RowStatistics`, the one maintained
  board statistic (see [[AI]] for why it is the only one).

## Bricks & AI

- **brickUtils** (`brickUtils.h/.cpp`) — brick shape/placement helpers operating on
  `gameObject` types.
- **AI** (`AI.h/.cpp`) — the enemy player: a bitboard beam search over hypothetical
  boards; depends on `gameObject` and `Timer` ([[Util]]). See [[AI]] for the design and
  its known gaps, and [[Attack Gauge]] for the system it will need to optimise.

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
- [[Entity Handles]] — how bricks and units are stored; the rule that keeps it sound
- [[Memory Safety]] — past heap corruption in this layer and how to reproduce it
- [[AI]] — the enemy player's design
- [[Attack Gauge]] — the core roguelike mechanic, not yet implemented
- [[Render]] — the GPU-facing types `Resources` assembles
- [[Util]] — `Timer`, `DebugLog`, `mathUtils` used throughout this layer
- [[Project Structure]] — overall layering rules
