---
tags: [tetris-roguelike, design, gameplay]
---

# Attack Gauge

The core roguelike mechanic. **Not implemented yet** — this note is the design intent,
captured 2026-09-08 from the first written description of it. Treat everything here as
a design target, not as a description of existing code.

Shorthand for the influences: **tetr.io's garbage system + Balatro's scoring**. The
whole game is meant to spin around optimising this gauge.

## The loop

1. Any attack generated **opens the gauge** if it is not already open.
2. While the gauge is open it *holds* the attack rather than sending it:
   - every further attack in the window **accumulates into the attack total**;
   - actions that pump the modifier **accumulate into the modifier**.
3. When the gauge **dies** (its window expires), `attack × modifier` is sent as garbage.

This is Balatro's chips × mult, but on a timer instead of a hand: the tension is how
long you can keep feeding a window before it closes.

## Garbage on the receiving side

Sent garbage does **not** immediately push rows into the enemy grid.

- It arrives **idle** — queued, visible as pending, but not yet part of the board.
- The enemy's own attacks **clear the idle garbage** before it lands. Their attack
  still runs the full gauge pipeline first, so countering is itself a timing decision,
  not a reflex.
- Idle garbage is **not idle forever**. After a delay it **materialises** into the
  enemy grid as real rows.

So each side is running two clocks at once: their own gauge window, and the
materialisation timer on whatever is pending against them.

## Why this shapes the AI

See [[AI]]. Two consequences:

- The evaluation function cannot be plain survival weights (aggregate height, holes,
  bumpiness). Those optimise for *not dying*; this game optimises for *gauge output*.
  Attack value, combo continuation, and window timing have to enter the score.
- Board state is no longer the whole state. An AI decision depends on the open gauge
  (how much is banked, how long the window has left) and on pending garbage (how much,
  how soon it materialises). The search's node state needs those fields, not just the
  bitboard.

Because the AI is specified as a **tunable-difficulty enemy**, the evaluation function
should be swappable and its weights data-driven from the start — difficulty scaling
across a run means varying search depth and weights, not rewriting the search.

## Open design questions

Unresolved, and each one changes the implementation:

- Does the gauge window have a **fixed duration**, or does a new attack **extend or
  reset** it? (Fixed = a pure race; extending = combo-chaining pressure.)
- What **pumps the modifier**? Spins, back-to-back, combos, perfect clears, roguelike
  items/relics — or only some of these?
- Does the modifier **persist across windows** or reset to ×1 each time?
- When enemy attack cancels idle garbage, is it **1:1 by line count**, and does it
  cancel **before or after** their own modifier is applied?
- When garbage materialises, how is the **hole column** chosen — fixed per volley,
  random per row, or telegraphed in advance while idle?
- Does materialising garbage that overflows the grid **end the run**, or is there a
  buffer?
- Where does the **roguelike progression** attach — relics that change gauge duration,
  modifier sources, cancel rules?

## See also
- [[World and Lore]] — why the gauge is the *magic's reading* of the cabinet's Tetris,
  not a feature of the cabinet; this is what licenses it to do un-arcade-like things
- [[Portal]] — how the player sees the opponent this gauge is aimed at, and the system
  waiting on this one to decide when a magical link opens
- [[AI]] — the enemy that has to play this system
- [[Game]] — where `AtackInfo` currently lives in `gameCore`
- [[Memory Safety]] — the attack-gap statistics that were an early attempt at this
