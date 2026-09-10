#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "render/Mesh.h"
#include "render/Light.h"
#include "render/point.h"
#include "render/animation.h"
#include "render/texture.h"
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

	glm::vec3 hexadecimalToRGB(std::string hex, float saturationOffset = 0.3);
	void load();

	Resources(std::vector<Shader>shds) : shaderProgram(shds)
	{
	}
};