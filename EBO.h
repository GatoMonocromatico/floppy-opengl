#pragma once

#include <glad/glad.h>
#include<vector>

class EBO
{
public:
	GLuint ID;
	EBO();
	void Bind();
	void Unbind();
	void Delete();
	void createEBO(std::vector<GLuint>& indices);
};
