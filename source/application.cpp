#include "application.h"

// load fragment file list
vector<fs::path> loadFragments(string fragmentSource)
{
	vector<fs::path> fragmentFileList;
	auto path = fs::path(fragmentSource);
	if (path.has_filename())
	{
		// check if it is a file list
		if (path.stem() == "*")
		{
			if (fs::exists(path.parent_path()))
			{
				copy_if(
					fs::directory_iterator(path.parent_path()),
					fs::directory_iterator(),
					back_inserter(fragmentFileList),
					[&](auto x) {return x.path().extension() == path.extension(); });
			}
		}
		else
			fragmentFileList.push_back(path);
	}

	// exit is file list is empty
	if (fragmentFileList.size() > 0)
	{
		if (fragmentFileList.size() > 1)
		{
			cout << "List of fragment files:" << endl;
			// print list of  in alphabetical order
			for (const auto& filename : fragmentFileList) {
				cout << filename.string() << endl;
			}
		}
	}
	else
	{
		cout << "Could not read fragment files! Check folder of extention is ini-file." << endl;
		exit(EXIT_SUCCESS);
	}

	return fragmentFileList;
}



Application::Application(Setup setup, string title): title(title)
{

	width = setup.width;
	height = setup.height;
	vertexSource = setup.vertexSource;
	fragmentFileList = loadFragments(setup.fragmentSource);

	mouseX = (double)width / 2.;
	mouseY = (double)height / 2.;
	numberFragments = (int)fragmentFileList.size();

	// initialize GLFW
	if (!glfwInit())
	{
		throw runtime_error("Could not initialize GLFW");
	}

/*
	// Tell GLFW what version of OpenGL we are using
	// in this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW that we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// antialiasing number of samples
	glfwWindowHint(GLFW_SAMPLES, 4);
*/

	// create a GLFW window of widthxheight pixels, named title
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	// Error check if window fails to create
	if (window == NULL) {
		cout << "Cannot create GLFW window! Exit..." << endl;
		exit(-1);
	}
	// introduce the window into the current context
	glfwMakeContextCurrent(window);

	// load GLAD to configure OpenGL 
	gladLoadGL();
	// specify the viewport of OpenGL in the Window
	// in this case the viewport goes from (0,0) to (800,600)
	glViewport(0, 0, width, height);

	// call backs
	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, window_resize);
	glfwSetKeyCallback(window, key_input_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);


	// configure global opengl state
	// enable the depth buffer (correct deep visualisation)
	glEnable(GL_DEPTH_TEST);

	// antialiasing multiple samples
	glEnable(GL_MULTISAMPLE); // enabled by default on some drivers, but not all so always enable to make sure

	// make alpha channel
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);

	prepareViewShader();
	compileShader();
	setTextures(setup.textureSource, setup.textureUniformName);

}

Application::~Application()
{
	// exit
	// destroy window before ending the program
	glfwDestroyWindow(window);
	// terminate GLFW before ending the program
	glfwTerminate();
}

void Application::window_resize(GLFWwindow* window, int width, int height)
{
	Application* handler = static_cast<Application*>(glfwGetWindowUserPointer(window));

	handler->width = width;
	handler->height = height;
	glViewport(0, 0, width, height);
	handler->compileShader();
}


void Application::key_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* handler = static_cast<Application*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (key == GLFW_KEY_RIGHT_BRACKET and action == GLFW_RELEASE)
	{
		handler->currectFragment++;
		if (handler->currectFragment >= handler->numberFragments)
			handler->currectFragment = 0;
		if (handler->numberFragments > 1)
			handler->compileShader();
	}
	if (key == GLFW_KEY_LEFT_BRACKET and action == GLFW_RELEASE)
	{
		handler->currectFragment--;
		if (handler->currectFragment < 0)
			handler->currectFragment = handler->numberFragments-1;
		if (handler->numberFragments > 1)
			handler->compileShader();
	}
}

void Application::mouse_move_callback(GLFWwindow* window, double  xpos, double  ypos)
{
	Application* handler = static_cast<Application*>(glfwGetWindowUserPointer(window));

	handler->mouseX = xpos;
	handler->mouseY = ypos;
}

// square window
vector<GLfloat> vertices = vector<GLfloat>({
	-1.f, -1.f, 0.f,
	 1.f, -1.f, 0.f,
	 1.f,  1.f, 0.f,
	-1.f,  1.f, 0.f,
	});
void Application::prepareViewShader()
{
	// Generate vertex array object and bind it 
	vao = new VAO();
	vao->Bind();
	// Generate vertex buffer object and link it to vertices 
	vbo = new VBO(vertices);

	// Link VBO to VAO
	vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, 0, NULL); // layout (location = 0) vertex coordinates

	//vao->Unbind();
	vbo->Unbind();
}

void Application::compileShader()
{
	string fragmentSource = fragmentFileList[currectFragment].string();
	new_title = title + " - " + fragmentFileList[currectFragment].filename().string();
	cout << "Compile fragment: '" << fragmentSource << "'" << endl;
	// set new window title
	glfwSetWindowTitle(window, new_title.c_str());
	glUseProgram(0);

	// create shaders
	shaderProgram = new Shader(vertexSource, fragmentSource);
	BindUniforms();
}

void Application::BindUniforms()
{
	shaderProgram->Activate();

	GLint resolutionLocation = glGetUniformLocation(shaderProgram->ID, "u_resolution");
	glUniform2fv(resolutionLocation, 1, value_ptr(glm::vec2(width, height)));

	GLint mouseLocation = glGetUniformLocation(shaderProgram->ID, "u_mouse");
//	glfwGetCursorPos(window, &mouseX, &mouseY);
	glUniform2fv(mouseLocation, 1, value_ptr(glm::vec2((float)mouseX, (float)height - (float)mouseY)));

	GLint timeLocation = glGetUniformLocation(shaderProgram->ID, "u_time");
	float time = (float)glfwGetTime();
	glUniform1f(timeLocation, time);
}

void Application::setTextures(string textureSource, string textureUniformName)
{
	// textures
	if (fs::exists(textureSource))
	{
		texture = new Texture(textureSource, GL_TEXTURE_2D, 0, GL_UNSIGNED_BYTE);
		texture->textureInit(shaderProgram, textureUniformName.c_str(), 0);
		// unbind to avoid accidental modification 
//		texture->Unbind();
	}
}

void Application::mainLoop()
{
	// main loop
	while (!glfwWindowShouldClose(window))
	{
		// take care of GLFW events
		glfwPollEvents();
		nbFrames++;
		showFPS();

		// specify the color of background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// clean the back buffer and assign new color to it
		// correct deep visualisation (GL_DEPTH_BUFFER_BIT)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// get time
		BindUniforms();

//		shaderProgram->Activate();
//		texture->Bind();
//		vao->Bind();
		// draw vertices
		glDrawArrays(GL_QUADS, 0, 4);

		// swap the back buffer with the front buffer 
		glfwSwapBuffers(window);
	}
}



void Application::showFPS()
{
	// Measure speed
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	// show FPS if last cout was more than 1 sec ago
	if (delta >= 1.0) {
		double fps = double(nbFrames) / delta;

		stringstream fps_title;
		fps_title << new_title << " [" << fixed << setprecision(2) << fps << " FPS]";

		glfwSetWindowTitle(window, fps_title.str().c_str());

		nbFrames = 0;
		lastTime = currentTime;
	}
}