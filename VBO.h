#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>


struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 diffuseUV;
	glm::vec2 specularUV;
	glm::vec3 normal;
	float intensity;
	/*Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texUV) :
		position(position),
		normal(normal),
		color(glm::vec3(1.0f, 1.0f, 1.0f)),
		texUV(texUV) 
	{}*/
};

class VBO
{
	public:
		GLuint ID;
		VBO();
		void Bind();
		void Unbind();
		void Delete();
		void createVBO(std::vector<Vertex>& vertices);
		void createVBO(std::vector<float>& data);
		void createVBO(std::vector<float>&& data);
};
