---
tags: [tetris-roguelike, architecture, game]
---

# Entity Handles

How bricks and brick units are stored, and the rule that keeps that storage sound.
Written 2026-09-08, when the pools were converted from compaction to free lists.

## The design being preserved

Every brick unit is a real `GameObject` entity, not a plain grid value. That is
deliberate: roguelike mechanics are meant to act on individual cells, so each cell has
to be addressable on its own. `GameObject` stays generic and carries a
`specificDataLocation` — a **handle** into a type-specific pool — rather than the union
of every specific type's fields.

The `model` / `animations` / `meshIDX` fields on a brick unit's `GameObject` look dead
today only because bricks currently render through a single instanced draw in
`drawGrid`. Per-cell mechanics will make them live. Do not "simplify" bricks into a
plain `Piece` grid on the strength of current usage.

## The pools

```
GridData
├── currentUnits  : std::vector<BrickUnitData>   pool, indexed by unit handle
├── freeUnits     : std::vector<size_t>          handles available for reuse
├── currentBricks : std::vector<BrickData>       pool, indexed by brick handle
├── freeBricks    : std::vector<size_t>          handles available for reuse
└── playingBrickHandle : int32_t                 the active piece
```

Handles are stored in `GameObject::specificDataLocation`, in
`BrickUnitData::indexInCurrentBricks`, and in `GridCell::unitHandle`.

## The rule

> **Pool entries are never moved and never renumbered.**

A handle stays valid for the entire lifetime of what it names. Freeing puts the slot on
the matching free list; the next allocation reuses it. `createUnit` / `freeUnit` /
`createBrick` / `freeBrick` in `brickUtils` are the only places that touch this.

The rule exists because handles are stored in many places at once. Moving an entry
obliges you to find and rewrite every handle naming it, and missing one is silent — the
stale handle simply addresses a different entry, or runs off the end. That is exactly
what happened: see [[Memory Safety]] for the three crashes it produced.

Growing a pool is still a reallocation, so the *other* rule still applies: **never hold
a `BrickUnitData&` or `BrickData&` across a call that can allocate.** Take the handle
and re-index. A free list makes handles stable; it does not make references stable.

## What this replaced

`updateGrid` used to compact `currentUnits` and rewrite handles in one pass, with an
accumulator that counted kept units where the arithmetic needed removed ones. It never
shrank the vector either, so the pool grew by 4 every piece, forever.
`eraseBricksFromCurrentBricks` did the same job for bricks. Both are gone.

`GridCell` used to reach a unit through two unstable indices,
`currentBricks[brickIndex].units[unitNum]`. It now holds one stable `unitHandle`, so a
cell is immune to both `BrickData::units` shrinking and the brick pool changing. This
made `BrickUnitData::numeration` and `GridCell::unitNum` dead, and they were removed.

## Ordering

The only ordering the game depended on was "which brick is the playing one", expressed
as `currentBricks.back()` — an implicit invariant every call site had to maintain, and
one that broke (see [[Memory Safety]] bug 2). It is now the explicit
`playingBrickHandle`, reached through `GridData::playingBrick()`.

If a mechanic later needs a real sequence — "the oldest landed brick decays", say — the
answer is a separate `std::vector<handle>` beside the pool, not position-as-order.
Storage owns identity and data; an order vector owns sequence. That way several
orderings can coexist over one pool, and none of them constrains how slots are reused.

## Optional hardening, not yet done

A stale handle currently reads whatever now occupies the slot. Making the handle
`{index, generation}` with a counter bumped on free turns that into a cheap detectable
failure. Worth adding if per-cell mechanics start holding handles across turns.

## See also
- [[Memory Safety]] — the crashes this design rule prevents, and how to reproduce them
- [[Game]] — the layer these pools live in
- [[Attack Gauge]] — the per-cell mechanics this identity exists to serve
