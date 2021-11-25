#include "VBO.h"

VBO::VBO(vector<GLfloat>& vertices)
{
	// generate VBO with only one object
	glGenBuffers(1, &ID);
	// bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	// introduce vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

}

VBO::~VBO()
{
	glDeleteBuffers(1, &ID);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
