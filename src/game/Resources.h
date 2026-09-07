#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "render/Mesh.h"
#include "render/Light.h"
#include "render/point.h"
#include "render/animation.h"
#include "render/texture.h"
#include "util/mathUtils.h"
#include <vector>
#include <array>

struct Resources
{
	Camera camera;

	GLuint brickUBO;
	GLuint scenarioUBO;

	std::vector<Light> brickLights;
	std::vector<Light> scenarioLights;

	std::array<std::vector<Texture>, 2> texs;

	std::vector< std::vector<Animation>> anims;
	std::vector< std::vector<Vertex>> verts;
	std::vector<std::vector<GLuint>> indices;

	std::vector<Mesh> meshs;
	std::vector<Point> points;

	std::vector<Shader> shaderProgram;

	glm::vec3 hexadecimalToRGB(std::string hex, float saturationOffset = 0.3);
	void load();

	Resources(std::vector<Shader>shds) : shaderProgram(shds)
	{
	}
};