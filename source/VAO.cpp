#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizei strife, GLsizei offset)
{
	VBO.Bind();

	// configure the vertex attribute, opengl will know how to extract them 
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, strife * sizeof(type), (void*)(offset * sizeof(type)));
	// enable vertex attribute for opengl
	glEnableVertexAttribArray(layout);

	//	VBO.Unbind();

}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

