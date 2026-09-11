#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "render/Mesh.h"
#include "render/Light.h"
#include "render/point.h"
#include "render/animation.h"
#include "render/texture.h"
#include "render/Framebuffer.h"
#include "render/Portal.h"
#include "util/mathUtils.h"
#include "util/StdAliases.h"
#include <vector>
#include <array>

struct Resources
{
	Camera camera;

	GLuint brickUBO;
	GLuint scenarioUBO;

	std::vector<Light> brickLights;
	std::vector<Light> scenarioLights;

	arrVec<Texture, 2> texs;

	stdMat<Animation> anims;
	stdMat<Vertex> verts;
	stdMat<GLuint> indices;

	std::vector<Mesh> meshs;
	std::vector<Point> points;

	std::vector<Shader> shaderProgram;

	// --- portal pipeline (see docs/Portal.md) -------------------------------
	// The opponent's board is rendered head-on into `opponentBoard` each frame;
	// `sceneCapture` snapshots the finished frame so the portal can refract what
	// is behind it; `portal` is the proxy quad the effect is marched through.
	Framebuffer opponentBoard;
	ScreenCapture sceneCapture;
	Portal portal;

	// Camera used only for the opponent-board pass. It looks at the same world
	// coordinates the grid is drawn at, head-on, because "the magic recreates a
	// plain view from the place" -- not a perspective-linked hole in space.
	Camera portalViewCamera;
	// Half-height, in world units, of the region the opponent-board pass frames.
	// That pass is orthographic (see Camera::updateMatrixOrtho), so this is a size
	// rather than a field of view and the camera's distance does not affect it.
	float portalViewHalfHeight = 0.38f;

	glm::vec3 hexadecimalToRGB(std::string hex, float saturationOffset = 0.3);
	void load();

	Resources(std::vector<Shader>shds) : shaderProgram(shds)
	{
	}
};