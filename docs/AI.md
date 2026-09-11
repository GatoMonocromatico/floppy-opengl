---
tags: [tetris-roguelike, ai, game]
---

# AI

The enemy player (`src/game/AI.h`, `AI.cpp`). Design decisions recorded 2026-09-08.
Its intended role is a **tunable-difficulty enemy** — strength is a dial that scales
across a run, not a fixed target.

## Why there is no incremental statistics pipeline

The original plan was to maintain board statistics (column heights, holes, attack gaps)
every frame so the AI would not have to recompute them each time it played. That plan
was dropped, and the half-finished pipeline deleted, because the premise does not hold:

**The AI never evaluates the live board.** It evaluates a few thousand *hypothetical*
boards per decision — every rotation × column, several pieces deep. Statistics
maintained on the live grid cannot be reused for any of them.

The costs are not where the plan assumed:

| | cost |
|---|---|
| Rebuilding the board snapshot from the grid | 26×10 = 260 cell reads, ~1 µs |
| One full beam search | well under 1 ms |
| Maintaining live statistics incrementally | a parallel structure, updated from several call sites, that must stay in sync through line clears and row shifts |

So the pipeline was paying real complexity to save time that was never being spent —
and it was the source of the heap corruption documented in [[Memory Safety]].

**What was kept:** `RowStatistics::numOfBricks`, a per-row occupancy count, because
`cleanFullLines` genuinely needs it to spot a full row in O(1). It is incremented in
exactly one place (a piece locking) and rewritten by the row shift. Everything else —
`ColumnStatistics`, `uniqueConsecutiveGaps`, `columnsAligningConsecutiveAtackGaps` —
was removed; those are board features, and board features belong inside `AISim`.

## Shape of the search

`AISim` is a bitboard: 26 rows of 10 bits plus a per-column top pointer. It is built
once per decision by `AISim::fromGrid`, excluding the falling piece, and every
hypothetical placement happens inside it. This is the right seam — the AI works in its
own representation and never touches the entity pools ([[Entity Handles]]).

On top of it, a beam search: for each node in the beam, try every rotation × column,
score the result, keep the best `beamWidth` and advance to the next look-ahead piece.
Weights are the Lee-style set (aggregate height, lines, holes, bumpiness, max height).
Beam search with 1–2 piece look-ahead and weights of this kind is the standard,
well-proven approach for Tetris AI — the basic choice is sound.

### Scoring must accumulate

`AINode::clearReward` carries the reward for lines cleared along the whole path;
`score = clearReward + boardEval(this node)`. Before this was fixed, `score` was
*overwritten* at every depth with the current node's evaluation, so a line cleared at
depth 0 was invisible in the final score and the search would discard clears outright.
Keep the two separate: line reward is cumulative down a path, board evaluation only
ever describes the board at that node.

## Known gaps

Ordered roughly by impact on play strength:

- **The hold piece is ignored.** `GridData::reservedBrick` exists and the AI never
  considers it. Large, cheap win.
- **Evaluation is survival-shaped, not attack-shaped.** Aggregate height / holes /
  bumpiness optimise for not dying. Per [[Attack Gauge]] this game is about maximising
  gauge output, so attack value, combo continuation, and window timing have to enter
  the score — and gauge state and pending garbage have to enter the search node
  alongside the bitboard. Make the evaluation swappable and its weights data-driven
  before tuning anything; tunable difficulty means varying depth and weights, not
  rewriting the search.
- **Only straight drops.** `calcLandingRow` drops a piece from above; no tucks under
  overhangs, no spins. Acceptable for now, but it caps strength.
- **Two rotation models.** `initAIPieceShapes` derives shapes by applying
  `createRotationalAdjustments` cumulatively, while `executeMove` replays the move
  through the real game's rotation and wall-kick system. These can silently diverge —
  the executor does not verify it reached the placement the search chose.
- **Frame-spreading is unnecessary complexity.** `workBudgetPerFrame` and the resumable
  iteration counters (`depth`/`nodeIdx`/`rotIdx`/`colIdx`) spread sub-millisecond work
  across frames. A synchronous search on piece spawn would delete the whole state
  machine. Only worth keeping if look-ahead depth grows a lot.

## See also
- [[Portal]] — grid 1 is no longer invisible: the AI's board is rendered every frame
  and shown to the player through the portal
- [[Attack Gauge]] — the system the evaluation function actually needs to optimise
- [[Entity Handles]] — the pools the AI reads from, and why it copies out instead
- [[Memory Safety]] — the corruption the deleted statistics pipeline was causing
- [[Game]] — the layer this lives in
