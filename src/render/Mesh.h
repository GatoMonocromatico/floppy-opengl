#pragma once

#include<string>
#include "util/StdAliases.h"
#include "render/VAO.h"
#include "render/EBO.h"
#include "render/camera.h"
#include "render/texture.h"

class Mesh
{
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		VBO VBO1;
		VBO VBOInstances;
		EBO EBO1;

		glm::mat4 model;


		VAO VAO1;
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture> textures, glm::vec3 model = glm::vec3(0.0f, 0.0f, 0.0f));
	void Draw(Shader& shader, Camera& camera, glm::mat4 objectModel = glm::mat4(1));
	void Draw(Shader& shader, Camera& camera, glm::mat4 objectModel = glm::mat4(1), int numIntaces = 1);
	void updateInstanceVBO(std::vector<float>& data);
	void translateModel(glm::vec3 translation);
	void rotateModel(glm::vec3 axis, float angle);
	void scaleModel(glm::vec3 scale);
	void createNormals();
	void Delete()
	{
		VAO1.Delete();
		VBO1.Delete();
		VBOInstances.Delete();
		EBO1.Delete();
		for (Texture& texture : textures)
		{
			texture.Delete();
		}
	}

};