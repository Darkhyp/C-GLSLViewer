#include <glad/glad.h>
#include <vector>

#pragma once

using namespace std;

class VBO
{
public:
	GLuint ID;

	VBO(vector<GLfloat>& vertices);
	~VBO();
	
	void Bind();
	void Unbind();
};

