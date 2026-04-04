#pragma once

#include <glad/glad.h>
#include<SDL3_image/SDL_image.h>

#include"shaderClass.h"

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLenum slot;

	Texture(const char* image, const char* texType, GLenum slot, GLenum format, GLenum pixelType);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};