---
tags: [tetris-roguelike, render]
---

# Render (`src/render/`)

Thin, game-agnostic OpenGL wrapper layer. Nothing here knows about grids, bricks, or
game state — see [[Game]] for that. Only depends on [[Util]] (`DebugLog.h`).

## Buffer objects

- **VBO** (`VBO.h/.cpp`) — Vertex Buffer Object, raw vertex data on the GPU.
- **EBO** (`EBO.h/.cpp`) — Element/Index Buffer Object, draw-order indices.
- **VAO** (`VAO.h/.cpp`) — Vertex Array Object; binds a `VBO` and records attribute
  layout so `Bind()` restores everything needed to draw without re-specifying pointers
  each frame. Depends on [[Render#Buffer objects|VBO]].

## Shading & textures

- **Shader** (`shaderClass.h/.cpp`) — compiles/links a vertex+fragment program from
  files under `assets/shaders/` (see [[Assets]]).
- **Texture** (`texture.h/.cpp`) — loads a 2D texture via SDL_image and binds it to a
  `GL_TEXTUREn` unit; wires a sampler uniform via `Shader`.
- **Light** (`Light.h`) — CPU-side mirror of the `Light` struct in `default.frag`
  (std140 layout) for UBO upload.

## Geometry

- **Mesh** (`Mesh.h/.cpp`) — a drawable made of vertices + indices + textures, built on
  `VAO`/`EBO`/`Texture`; also depends on `Camera` for drawing with a view/proj matrix.
- **Point** (`point.h/.cpp`) — minimal `GL_POINTS` draw path (debug markers, particles),
  same `Vertex` layout as `Mesh` but drawn with `glDrawArrays` instead of indexed
  triangles.
- **RoundedCorner2D** (`RoundedCorner2D.h/.cpp`) — generates rounded-rect 2D geometry as
  a `Mesh`.

## Camera & animation

- **Camera** (`camera.h/.cpp`) — view matrix, optional FPS yaw/pitch, uploads
  `cameraMatrix` (proj * view) to shaders.
- **Animation** (`animation.h`) — sprite-sheet frame stepping; depends on `Timer` from
  [[Util]].

## Dependency shape

```
Mesh, Point ── depend on ──> VAO, EBO, Camera, Texture
VAO ─────────> VBO
Camera, Texture, Shader ─────> DebugLog (util)
```

## See also
- [[Game]] — the layer that assembles these into `Resources` and draws the actual game
- [[Assets]] — where the shader source files and texture images these classes load
  actually live
