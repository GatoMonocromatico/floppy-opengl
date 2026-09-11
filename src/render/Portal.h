#pragma once
#define GLM_ENABLE_EXPERIMENTAL
// -----------------------------------------------------------------------------
// Portal.h -- the magical window that shows another arcade's board
// -----------------------------------------------------------------------------
// Owns proxy geometry (one quad) and the uniform feed for portal.frag, which
// raymarches the whole effect in the portal's own local space. See docs/Portal.md
// for the visual design; this file only covers the mechanics.
//
// Local space is deliberately ISOTROPIC: the quad's half-extents are baked into
// its vertex positions in world units, and `model` is kept to rotation +
// translation (+ uniform scale) only. A non-uniform scale in `model` would skew
// the ray directions the fragment shader builds, and the black hole would shear
// as the camera moved.
//
// The quad is larger than the window rectangle it paints, because the matter's
// accretion arms reach past the board -- see `matterMargin`. Proxy geometry that
// only covered the board would clip the effect's own silhouette.

#include "util/DebugLog.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/VAO.h"
#include "render/VBO.h"
#include "render/EBO.h"
#include "render/camera.h"
#include "render/shaderClass.h"

class Portal
{
public:
	// Half-extents, in world units, of the rectangle the opponent's board is
	// painted into. The board image keeps this aspect; the matter ignores it.
	glm::vec2 windowHalf = glm::vec2(0.21f, 0.42f);
	// Outer reach of the matter, in world units. This is the ONE size knob for
	// the effect: portal.frag works in ratios of it, so the horizon, the band and
	// the falloffs all scale together from here. The proxy quad is sized to
	// contain it, because geometry smaller than the silhouette would clip the
	// effect's own edge.
	float matterRadius = 0.58f;
	// Slack between the matter's reach and the quad's edge, so the silhouette's
	// soft falloff has somewhere to fade out.
	float quadSlack = 1.10f;

	glm::vec2 quadHalf() const
	{
		return glm::vec2(glm::max(windowHalf.x, matterRadius),
		                 glm::max(windowHalf.y, matterRadius)) * quadSlack;
	}

	// Fraction of the content target that the opponent's grid occupies, per axis.
	// The target deliberately holds a margin of their screen background around the
	// grid: a rectangle inscribed in the matter's ellipse leaves spare area at its
	// sides, and that background is what fills it. See docs/Portal.md.
	glm::vec2 contentScale = glm::vec2(1.0f);

	// --- placement ------------------------------------------------------------
	// Set these and call setTransform(). `scale` is uniform and multiplies the
	// whole effect: the matter, the window, the arcs' thickness and the
	// materialisation's grain all grow together, because every length in
	// portal.frag is expressed in this object's local units. Local space must stay
	// ISOTROPIC (see the note at the top of this file), so there is deliberately no
	// per-axis scale -- to change the window's proportions, change windowHalf.
	glm::vec3 position = glm::vec3(0.0f);
	float yawDegrees = 0.0f;
	float pitchDegrees = 0.0f;
	float scale = 1.0f;

	glm::mat4 model = glm::mat4(1.0f);

	// Phase accumulators for the animation. Kept on the CPU as doubles and
	// uploaded wrapped, so a long session never loses shader-side float
	// precision -- see stepTime().
	double swirlPhase = 0.0;
	double driftPhase = 0.0;
	double flickerPhase = 0.0;

	// 0 = fully dematerialised, 1 = fully present. Drives the spawn transition,
	// which portal.frag renders as a ragged noise dissolve with a burning edge
	// rather than a flat fade. Advanced by stepTime() while a transition runs.
	float materialisation = 1.0f;
	float materialiseDuration = 1.8f;
	// Direction of the running transition: +1 opening, -1 closing, 0 settled.
	float materialiseDirection = 0.0f;

	void beginMaterialisation() { materialisation = 0.0f; materialiseDirection = 1.0f; }
	void beginDematerialisation() { materialiseDirection = -1.0f; }

	Portal() = default;

	// Builds (or rebuilds) the proxy quad. Safe to call again after changing
	// windowHalf / matterRadius / quadSlack: it re-uploads the geometry when those
	// change. It used to early-out on a `built` flag, which meant a later size
	// change updated the shader's uniforms while leaving the proxy geometry stale,
	// and the effect was quietly clipped by its own quad.
	void create();
	// Advances the animation phases. Rates are mutually irrational multiples so
	// the combined field never returns to a previous state -- no visible loop.
	void stepTime(float deltaTime);
	// Rebuilds `model` from position / yawDegrees / pitchDegrees / scale.
	void setTransform();
	// Convenience: assigns the placement fields, then rebuilds.
	void setTransform(glm::vec3 pos, float yaw, float pitch, float uniformScale = 1.0f);

	// contentTex: the opponent's board (Framebuffer::colorTex).
	// backgroundTex: the already-drawn scene, for the matter's refraction.
	void Draw(Shader& shader, Camera& camera, GLuint contentTex, GLuint backgroundTex,
	          const glm::vec2& viewportSize);
	void Delete();

private:
	VAO vao;
	VBO vbo;
	EBO ebo;
	bool built = false;
	glm::vec2 builtQuadHalf = glm::vec2(0.0f);
};
