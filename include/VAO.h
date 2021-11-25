#include<glad/glad.h>
#include "VBO.h"

#pragma once

class VAO
{
public:
	GLuint ID;

	VAO();
	VAO(const VAO&) = delete; // interdiction to copy
	~VAO();

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, GLsizei offset);
	void Bind();
	void Unbind();
	void Delete();
};

