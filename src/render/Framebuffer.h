#pragma once
// -----------------------------------------------------------------------------
// Framebuffer.h -- offscreen colour target (+ depth) for render-to-texture
// -----------------------------------------------------------------------------
// Two users, both in the portal pipeline (see docs/Portal.md):
//   * the opponent-board pass renders grid 1 into `colorTex`, which the portal
//     shader samples as the image the magic paints onto the matter;
//   * the scene-capture pass copies the already-drawn frame so the portal can
//     refract what is behind it.
//
// The colour attachment is a plain GL_RGBA8 texture with CLAMP_TO_EDGE wrapping:
// the portal samples it with distorted coordinates that routinely fall outside
// [0,1], and GL_REPEAT there would wrap the opponent's board around itself.

#include "util/DebugLog.h"
#include <glad/glad.h>

class Framebuffer
{
public:
	GLuint ID = 0;
	GLuint colorTex = 0;
	GLuint depthRBO = 0;
	int width = 0, height = 0;

	Framebuffer() = default;

	// Allocates the FBO and its attachments. Safe to call once per target.
	void create(int w, int h);
	// Reallocates attachments when the requested size changed; no-op otherwise.
	void resize(int w, int h);
	void Bind();
	// Rebinds the default framebuffer. Callers must restore their own viewport.
	static void Unbind();
	void Delete();
};

// A colour texture filled by copying out of whatever is currently drawn on the
// default framebuffer. The portal needs the scene BEHIND it in order to refract
// it, and the scene is only complete once everything else has been drawn -- so
// the frame is drawn normally, snapshotted here, and the portal is drawn last on
// top while sampling the snapshot.
//
// This is a copy rather than a render-to-texture pass because it costs one
// GPU-side blit and needs no format matching against the window's own buffers.
class ScreenCapture
{
public:
	GLuint texture = 0;
	int width = 0, height = 0;

	// Sizes (or resizes) the texture and copies the current framebuffer into it.
	void capture(int w, int h);
	void Delete();
};
