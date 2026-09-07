---
tags: [tetris-roguelike, util]
---

# Util (`src/util/`)

Dependency-free leaves — nothing here includes from [[Render]] or [[Game]], so they can
be included from anywhere without pulling in the rest of the project.

- **DebugLog** (`DebugLog.h`) — `DBG`/`MDBG` logging macros, compiled in only when the
  `DEBUG` define is set (Debug builds only, see [[Build System]]).
- **Timer** (`Timer.h`) — countdown/elapsed-time helper used by `Animation` ([[Render]])
  and `AI`/`GameState` ([[Game]]).
- **mathUtils** (`mathUtils.h/.cpp`) — small standalone math helpers.

## See also
- [[Render]] and [[Game]] — both layers depend on this one, never the reverse
