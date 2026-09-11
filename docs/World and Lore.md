---
tags: [tetris-roguelike, design, lore]
---

# World and Lore

The fiction the game is built inside, captured 2026-09-09 from the first written
description of it. Nothing here is implemented as *story* — there is no written lore
yet — but it is the rule set that decides what every feature is allowed to look like,
so it belongs next to the code rather than in someone's head.

## The one rule

> **The player is a person in a room, not a player at a menu.**

Everything follows from this. The game is 3D because a person can walk away from an
arcade machine, look at it from the side, or stand back from it. Any feature that would
only make sense to someone holding a controller has to be re-expressed as something
that could happen to a person standing in that room.

## The two layers: the machine, and the magic

There are two entirely separate sources of behaviour, and keeping them separate is what
keeps the fiction coherent.

**The arcade machine** provides the base game and nothing else. It is an ordinary
cabinet running ordinary Tetris. It is single-player, or at best local multiplayer.
It has no idea anyone else exists. Clearing a line on it does what clearing a line on
an arcade machine does: it clears the line.

**The magic** is the second layer, and it is what makes this a competitive game. The
arcade is magical — how and why is undeveloped — and the magic *connects multiple
arcades together*. It watches what happens on each machine and interprets it:

- Clearing lines has **no effect inside the game itself**.
- The magic observes the clear, and *calculates an attack from it*.
- That attack is delivered to another player's machine by the magic, not by the game.

So [[Attack Gauge]] is not a feature of the Tetris the cabinet is running. It is the
magic's reading of that Tetris. This matters when designing it: the gauge can do
things a 1985 arcade board could never do, because it is not the arcade board doing
them.

Competition format is open — the current target is one player against offline AI
([[AI]]), with real multiplayer a distant possibility.

## Portals: how you see the other player

Modern Tetris shows you the opponent's board in a corner of the screen. An arcade
cabinet meant for one person has no corner to put that in, and inventing one would
break the one rule above.

The magic's answer is a **portal**: a window, hanging in the room, that shows the other
place. See [[Portal]] for what is actually built.

The important design constraint, and it is deliberate:

> A portal is a **recreated plain view** of the other place, not a physical hole
> between two rooms.

It does **not** work like a real opening in space, where moving your head would change
the angle you see through it. The magic reconstructs a flat view of the other board and
shows it to you. That is a simplification, but it is a *diegetic* simplification —
the magic is drawing you a picture, and a picture does not have to obey parallax.
It also keeps the opponent's board legible from anywhere in the room, which a true
perspective portal would not.

## Making the magic look like magic

The visual grammar for anything magical, in one line: **a chameleon that is not quite
good enough.**

- The magic tries to hide itself in its surroundings. Most of what you see through a
  magical effect is just what is behind it, or what it is showing you.
- It fails in small, specific ways — a lens-like distortion, matter that does not quite
  match its background, a glow where the magic is concentrated.
- Those failures are the only thing that tells you it is there.

The corollary is that magical effects must **not** demand attention. They live on
screen for the whole match. An effect that is spectacular the first time is intolerable
by the fiftieth.

## Other places the magic shows up

Undeveloped, but recorded so they are designed to the same grammar:

- **3D pieces.** An option to have the tetrominoes come out of the screen as real 3D
  objects rather than sitting flat on the monitor. This is magic too, but it is *not*
  meant to carry a permanent visual effect — turning it on or off plays a transition,
  and after that the pieces are simply out of the screen.
- **Settings as a magical menu.** Settings screens are not UI. They are a place where
  the player manipulates magic directly. The shape of that is unresolved, but the
  requirement is fixed: it must read as a person handling something, not as a person
  operating a menu.

## See also
- [[Portal]] — the first magical effect actually built, and the design grammar applied
- [[Attack Gauge]] — the mechanic the magic computes from what the cabinets are doing
- [[AI]] — the opponent on the other end of a portal
- [[Game]] — the layer the cabinet's own Tetris lives in
