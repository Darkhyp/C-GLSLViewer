#include <string>
#include <fstream>
#include <iostream>
//#include <sstream>
//#include <cerrno>
#include<glad/glad.h>

#pragma once

using namespace std;

string get_file_contents(string filename);

class Shader
{
public:
	GLuint ID;

	Shader(string vertexfile, string fragmentfile);
	~Shader();

	void Activate();
	void CompileErrors(GLuint shader, const char* type, string filename);
};

