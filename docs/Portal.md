---
tags: [tetris-roguelike, render, design]
---

# Portal

The magical window that shows another arcade's board. Built 2026-09-09. This is the
first magical effect in the game, so it also sets the visual grammar described in
[[World and Lore#Making the magic look like magic]].

## What it is meant to look like

The reference is a lensed black hole (`ideas/portal/black-hole-effect.mp4`), but only
two things are taken from it: the **diagonal, lens-shaped silhouette** and the
**moving turbulent texture** of the disk. The reference's orange fire is explicitly
*not* taken, and neither is its geometry — in the video that silhouette comes from
viewing a disk edge-on, which turns out to be exactly the wrong way to build it here
(see [[Portal#The matter is a slab lying in the window's plane]]).

The mental model for the rest:

> The magical matter is a **canvas**. The magic paints the other player's board onto
> it. The matter is still an object with its own properties, so the paint is not
> perfectly flat and the canvas shows through.

Which gives three layers, composited in `assets/shaders/portal.frag`:

1. **Matter** — a raymarched slab of spinning matter. It supplies the
   silhouette and the swirling structure. Its body mostly **refracts whatever is
   behind the portal**, which is what makes it blend into the room rather than
   announce itself.
2. **Window** — the opponent's board, painted on as a rectangle. It covers most of the
   matter, leaving two opposing lobes exposed.
3. **Arcs** — the matter is elliptical and the board is rectangular, so the ellipse
   cannot reach all four corners. Two opposing corners fall outside it. There, a thin
   electric filament stands in for the missing edge.

Teal/cyan is the magic's colour. It appears **patchily**, where the magic happens to be
concentrated, not as a uniform rim.

## The matter is a slab lying in the window's plane

The single most important geometric decision, and it was got wrong first.

The slab lies **nearly in the window's own plane** -- about 11 degrees out of it --
and it is an **ellipse**, not a circle, elongated along the window rectangle's
diagonal. It has real thickness (0.20 of its semi-major axis), so it is a lens of
matter rather than a sheet.

It originally sat ~67 degrees from the window's plane, near perpendicular. That
reproduces the reference video's edge-on lens shape when you face the portal --
but the video has a fixed camera and no window. Glued to one, that geometry means
walking to the side foreshortens the window to a line while simultaneously turning
you to face down the disk's axis, so the matter **opens into a full circle exactly
as the window vanishes**. Both silhouettes are individually correct for a rigid
object; the pairing is nonsense. A picture frame with a wheel through it edge-first
is not a thing, so the eye reads two unrelated effects rather than one object, and
the whole illusion collapses into "a vfx".

Laying the slab down fixes it: the matter and the window now foreshorten *together*
from every angle. The cost is that the elongated silhouette can no longer come from
foreshortening, so it has to come from the body's own shape -- hence the ellipse.
The lensed over/under arcs are gone too; those need an edge-on disk.

### Why the corners work out

An ellipse elongated along one diagonal of a rectangle covers the two corners on
that diagonal and misses the other two. The projected silhouette is about
`0.56 x 0.31` against a `0.21 x 0.42` window: wide enough to cover both short edges
and both long edges, too narrow to reach the off-diagonal corners.

The arcs are then derived, never authored: `arc = (on the window border) AND (matter
does not cover here)`. They follow the silhouette automatically, so retuning the
matter moves the arcs to match rather than desynchronising from them. Their
brightness ramps with true distance outside the silhouette, so a corner that has
only just cleared the matter barely glows and the corner tip is brightest.

Traps this walked into, all worth remembering:

- **The silhouette is not the density.** The matter is an *annulus*, so its density
  falls off towards the middle -- and the window's side edges run right through that
  inner hole. Deriving "is the border exposed here" from density reported bare
  border down the entire left and right sides and the arcs wrapped the whole
  rectangle. `Matter::body` answers the coverage question separately, ignoring the
  inner cut and the turbulence.
- **Ramp distance must match the corners' real reach.** The corner sits about 0.10
  world units outside the silhouette; a longer ramp than that means the arc never
  brightens anywhere along its visible length.
- **The arc core must stay below saturation.** At full brightness it clipped to
  white along its whole length and the travelling pulses became invisible -- the arc
  animated, but nothing about it could be seen to move.

## Depth ordering, and why the naive version fails

Matter the ray meets before it reaches the window must end up **in front of** the
board. Without that the board is composited over the matter unconditionally and the
matter can never occlude it, whatever the camera does.

The obvious cheap test -- classify each sample by the sign of its local z -- is
**degenerate here and measured exactly zero back-side density**. The slab is centred
*on* the window plane and its projected major axis *lies in* that plane, so which
side a sample falls on flips arbitrarily precisely along the axis, which is where
the visible lobes are.

`marchMatter` therefore composites front-to-back along the ray and drops the board
in at the depth the ray actually reaches the window (`tWindow`). Depth along a ray
has no such degeneracy. Two things fall out of it for free:

- Matter already past the board emits at full strength; matter in front of it is
  damped to `frontGlow`, but only where there *is* a board to occlude -- damping it
  everywhere also dimmed the exposed lobes, where nothing is being hidden.
- Absorption is Beer-Lambert per step. A grazing ray travels much further through
  the same matter and goes smoothly opaque; a clamp instead flat-topped into the
  uniform grey wash that oblique views used to show.

**The march accumulates PREMULTIPLIED**, so the portal is drawn with
`glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)`. Under ordinary `GL_SRC_ALPHA`
blending the same value is multiplied by alpha a second time and the entire effect
comes out roughly half as bright -- which looked like "the matter disappeared".

### The hole has to be filled

The window sits at the centre of the annulus's hole, and **a hole is unobstructed
from every direction**, so no amount of slab geometry could ever occlude the window
on its own. `innerHaze` fills it with thin material. It needs no view-dependent term
to do its job: it lies in the same slab, so a face-on ray crosses it almost
instantly and it stays invisible, while a grazing ray travels along its length and
Beer-Lambert turns that into real opacity.

## Never looping

The animation must not visibly repeat — a loop that is invisible in the first minute
is intolerable after an hour of play. Two independent measures:

- **Differential rotation.** Matter closer to the centre orbits faster
  (`swirlPhase * keplerRate / rr^1.5`), so the field shears forever and never returns
  to an earlier configuration. This is structural, not a long baked animation.
- **Three phase accumulators** at mutually irrational rates (`1/phi`, `sqrt(2)-1`,
  `sqrt(3)`), making the combined field quasi-periodic.

There is a second, quieter version of the same problem: a `time` uniform that just
counts seconds loses float32 mantissa and the swirl visibly quantises after an hour or
so. The phases are therefore accumulated as **doubles on the CPU** and wrapped to a
fixed period before upload (`Portal::stepTime`).

## Where the image comes from

`renderOpponentBoard` in `main.cpp` renders grid 1 -- which the [[AI]] has been
playing all along, and which was previously never drawn -- into an offscreen target
each frame.

- The projection is **orthographic**. Under perspective the brick cubes near the
  frame's edges are seen at an angle, show their dark side faces and clip the
  frustum, so the outer columns and rows read blunt next to the middle ones. Ortho
  presents every cube's front face identically. It also renders the opponent's
  pieces effectively **flat**, without needing separate flat geometry -- which is
  what the cabinet is supposed to draw anyway, with 3D pieces being the magic layer
  on top rather than something the machine itself does.
- It draws the **cabinet art as well as the bricks**, and frames a small margin of
  the opponent's screen around the playfield. The margin does two jobs: the grid is
  inset inside the window (`boardInset`) so the outermost cells sit on a band of
  their screen rather than flush against the edge, and the surplus fills the spare
  area the matter's ellipse leaves at the rectangle's sides. It is filler, not
  subject -- a generous margin pulls their NEXT panel into frame and, masked across
  the whole ellipse, paints over the matter's swirls entirely.
- Only rows 6..25 are framed. Rows 0..5 are the spawn buffer, and showing them would
  leak the opponent's next piece before it enters play.
- The view camera is tilted by the same -8 degrees as the board mesh, so the board is
  square to it and the image is not a trapezoid.

A brick's own quad spans `[0, cellSize]` in x but is **centred** in y, so the two
axes centre differently. Getting this wrong shifts the frame half a cell sideways
and pulls the well's border into view.

## Rendering order

```
1. renderOpponentBoard   -> opponentBoard FBO      (binds its own viewport)
2. the scene             -> default framebuffer
3. sceneCapture.capture()                          (snapshot for refraction)
4. portal.Draw()         -> over the captured frame, depth writes off,
                            PREMULTIPLIED blending
```

The portal is last because it refracts the finished frame. Anything drawn after it is
simply not part of what the matter can bend. Depth writes are off because it is a
transparent volume — letting its proxy quad write depth would have the quad occlude
the glow it sprays beyond the window rectangle.

The room's clear colour was changed from white to near-black at the same time: the
matter mimics what is behind it, and a white void gives it nothing to mimic.

## Code shape

- **`render/Portal.h/.cpp`** — proxy quad, transform, phase accumulators, uniform feed.
  Local space is deliberately **isotropic**: the quad's half-extents are baked into its
  vertex positions in world units and `model` is kept to rotation + translation +
  *uniform* scale. Non-uniform scale there would skew the ray directions the fragment
  shader builds and the black hole would shear as the camera moved.
- **`render/Framebuffer.h/.cpp`** — `Framebuffer` (colour + depth render target) and
  `ScreenCapture` (copy of the default framebuffer). Both clamp to edge, because the
  portal samples them with distorted coordinates that routinely leave `[0,1]`.
- **`assets/shaders/portal.vert`** — hands the fragment its local position; the effect
  is entirely per-fragment.
- **`assets/shaders/portal.frag`** — everything else.

### Sizing and placement

All of it lives in one labelled block in `Resources::load()`, and there are two
different kinds of knob:

- **`Portal::scale`** resizes the whole thing. It is uniform, and it multiplies the
  matter, the window, the arcs' thickness and the materialisation's grain together,
  because every length in `portal.frag` is expressed in the portal's local units.
  This is the one to reach for. There is deliberately no per-axis scale: local space
  must stay isotropic, or the ray directions the fragment shader builds are skewed
  and the body shears as the camera moves.
- **`windowHalf` and `matterRadius`** change the effect's *proportions*. They
  interact, and not freely: the matter's ellipse has to cover the window's short
  edges and two opposing corners while falling short of the other two, or the
  electric arcs have nothing to stand in for. Retune them together.

`Portal::create()` bakes `quadHalf()` into the proxy geometry, so it **rebuilds when
those local dimensions change**. It used to early-out on a `built` flag, which meant
a later size change updated the shader's uniforms while leaving the proxy quad stale,
and the effect was quietly clipped by its own geometry.

`TR_PORTAL="x,y,z[,yaw,pitch,scale]"` overrides placement and scale at startup, so
trying a size is a rerun rather than a rebuild. Every other tuning value is a `const`
in the fragment shader, and shaders are read from disk at startup, so the look can be
retuned by editing that file and rerunning — **no C++ rebuild** either way.

## Materialisation

The portal condenses into existence rather than fading in: a noise field is compared
against a rising threshold, so the reveal front is ragged, and the leading edge glows.
`Portal::beginMaterialisation()` / `beginDematerialisation()` drive it; `M` replays it.

`assets/textures/enemy-portal-spawn-anim.png` is the hand-drawn version of this same
idea (36 frames of a splatter blob growing into a plain rectangle). It is currently
**unused**: it is an alpha mask whose final frame is a rectangle smaller than the
proxy quad, so multiplying the portal by it would clip the matter's lobes, and its
frames are a different aspect from the window. The procedural dissolve needs no atlas
alignment and does not loop. The texture is worth revisiting if the hand-drawn shape is
wanted specifically.

That animation used to be attached to `scenarioObjects[1]`, which pointed at
`meshIDX` 2 — the brick-unit cube, not the portal quad. It therefore rendered a stray
cube at the bottom of the playfield and the animation was never visible at all. The
portal replaces that draw.

## Known gaps

- The portal is a fixed object in the room. Nothing spawns or despawns it yet, because
  [[Attack Gauge]] — the system that would decide when a magical link opens — does not
  exist.
- Seen from behind, the board is still painted and readable (mirrored). A real view
  would not be.
- At true grazing angles the whole portal foreshortens to almost nothing. That is
  the accepted consequence of laying the slab down: matter and window collapse
  together, which is what makes it read as one object, but it means there is little
  to look at from directly beside it.
- The arcs' silhouette ellipse is analytic and does not change with the viewpoint,
  while `Matter::body` (which gates them) is marched and does. The two agree closely
  now that the slab lies in the window's plane, but they are still two descriptions
  of one shape.
- `drawGrid` interpolates the falling piece using `gs.gravityTimer` and
  `gs.coutingLockIn`, which are the **player's** timers. The opponent's falling piece
  is therefore interpolated against the wrong clock — visible only as slightly wrong
  sub-cell motion on their active piece.
- Only one portal exists. Several would each need their own target and capture.

## See also
- [[World and Lore]] — why a portal is a recreated flat view and not a hole in space
- [[Render]] — the layer `Portal` and `Framebuffer` live in
- [[AI]] — the opponent whose board is on the other side
- [[Attack Gauge]] — what will eventually decide when portals open
- [[Assets]] — the shader and texture files this loads
- [[Build System]] — `TR_CAMERA` / `TR_SHOT`, the hooks used to verify this
