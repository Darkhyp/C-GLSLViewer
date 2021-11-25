#include <glad/glad.h>
#include "shader.h"

#pragma once

class Texture
{
public:
	GLuint ID;
	GLenum type;
	GLuint unit;

	Texture(string image, GLenum textureType, GLuint slot, GLenum pixelType);
	~Texture();

	void textureInit(Shader* shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();

};

