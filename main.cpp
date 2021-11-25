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

	// load file list
	vector<std::filesystem::path> fragmentFileList;
	if (fs::is_directory(setup.fragmentSourceFolder)) {
		copy_if(
			fs::directory_iterator(setup.fragmentSourceFolder),
			fs::directory_iterator(),
			back_inserter(fragmentFileList),
			[&](auto x) {return x.path().extension() == setup.fragmentSourceExtension; });
	}	
	if (fragmentFileList.size() > 0)
	{
		cout << "List of fragment files:" << endl;
		// print list of  in alphabetical order
		for (const auto& filename : fragmentFileList) {
			cout << filename.string() << endl;
		}
	}
	else
	{
		cout << "Could not read fragment files! Check folder of extention is ini-file." << endl;
		exit(EXIT_SUCCESS);
	}
	

	// ******************** main application ************************
	cout << "Shader viewer. View shaders from folder '" << setup.fragmentSourceFolder << "'" << std::endl;

	Application app(setup.width, setup.height, "Shader viewer", setup.vertexSource, fragmentFileList);

	app.mainLoop();

	return 0;
}
