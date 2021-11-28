#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "VAO.h"
#include "texture.h"
#include "application.h"
#include "setup.h"

#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

int main(int argc, char **argv) {
	// check external arguments
	string iniFileName;
	if (argc == 2)
	{
		cout << "Load values from " << argv[1] << " file" << endl;
		iniFileName = string(argv[1]);
	}
	else
	{
		cout << "ini-file is not specified. Using default [setup.ini]" << endl;
		cout << "Use: shaderview.exe [ini-file]" << endl;
		iniFileName = string("setup.ini");
	}

	Setup setup(iniFileName);
	setup.load();

	// ******************** main application ************************
	cout << "Shader viewer" << std::endl;

	Application app(setup, string("Shader viewer"));

	app.mainLoop();

	return 0;
}
