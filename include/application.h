#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "VAO.h"
//#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;


#pragma once

class Application
{
public:
	GLFWwindow* window;
	int width, height;
	const char* title;

	Shader* shaderProgram;

	VAO* vao;
	VBO* vbo;
	//Texture* cubeTexture, *cubeSpecularMap;

	// vertex file
	string vertexSource;

	// fragment files list
	vector<fs::path> fragmentFileList;
	int currectFragment = 0;
	int numberFragments;

	// mouse positions
	double mouseX, mouseY;


	Application(int width, int height, const char* title, string vertexSource, vector<fs::path> fragmentFileList);
	~Application();

	static void window_resize(GLFWwindow* window, int width, int height);
	static void key_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_move_callback(GLFWwindow* window, double  xpos, double  ypos);

	void prepareViewShader();
	void compileShader();

	void BindUniforms();
	void mainLoop();
};

