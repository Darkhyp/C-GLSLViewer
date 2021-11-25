#include "shader.h"

// read a text file and outputs a string
string get_file_contents(string filename)
{
	ifstream in(filename, ios::binary);
	if (in)
	{
		string fcontent((istreambuf_iterator<char>(in)), (istreambuf_iterator<char>()));
		in.close();
		return fcontent;
	}
	cout << "Could not open shader file: " << filename << "! Exit..." << endl;
	//throw(errno);
	exit(EXIT_FAILURE);
}


GLuint compileShader(string shaderfile, GLenum type)
{
	string shaderCode = get_file_contents(shaderfile);
	const char* shaderSource = shaderCode.c_str();

	// create shader object and get it reference
	GLuint shader = glCreateShader(type);
	// attach shader source to the vertex shader object
	glShaderSource(shader, 1, &shaderSource, NULL);
	// compile the shader to machine code
	glCompileShader(shader);

	return shader;
}

Shader::Shader(string vertexfile, string fragmentfile)
{
	// create vertex shader object and get it reference
	GLuint vertexShader = compileShader(vertexfile, GL_VERTEX_SHADER);
	CompileErrors(vertexShader, "VERTEX", vertexfile);

	// create fragment shader object and get it reference
	GLuint fragmentShader = compileShader(fragmentfile, GL_FRAGMENT_SHADER);
	CompileErrors(fragmentShader, "FRAGMENT", fragmentfile);

	// create shader program
	ID = glCreateProgram();
	// attach the vertex and the fragment shaders to the shader program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// link/wrap-up all the shader together into the shader program
	glLinkProgram(ID);
	CompileErrors(ID, "PROGRAM", vertexfile);

	// delete now useless vertex and the fragment shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::Activate() 
{
	glUseProgram(ID);

}

void Shader::CompileErrors(GLuint shader, const char* type, string filename)
{
	GLint log_length;
	GLint hasCompiled;
	string infoLog;
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if(hasCompiled == GL_FALSE)
		{
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
			infoLog.resize(log_length);
			glGetShaderInfoLog(shader, log_length, NULL, &infoLog[0]);
			cout << "SHADER COMPILATION ERROR for " << filename << endl;
			cout << infoLog << endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &log_length);
			infoLog.resize(log_length);
			glGetProgramInfoLog(shader, log_length, NULL, &infoLog[0]);
			cout << "SHADER LINKING ERROR for " << filename << endl;
			cout << infoLog << endl;
		}
	}
}
