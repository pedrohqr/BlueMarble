#include <iostream>
#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const int Height = 600;
const int Width = 800;

int main() {
	//initializing GLEW lib
	assert(glfwInit() == GLFW_TRUE);

	//creating the main window
	GLFWwindow* MainWindow = glfwCreateWindow(Width, Height, "BlueMarble - Pedro Ramos", nullptr, nullptr);
	assert(MainWindow);

	//assign the glew context to current window
	glfwMakeContextCurrent(MainWindow);

	//initialize GLEW lib
	assert(glewInit() == GLEW_OK);

	//verify especs of the driver and the GPU
	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;

	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);

	std::cout << "OpenGL Version : " << GLMajorVersion << "." << GLMinorVersion << std::endl;
	std::cout << "OpenGL Vendor : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GPU: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GLSL Version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	//assign background color
	glClearColor(0.0f, 0.1f, 0.0f, 1.0f);

	//events queue
	while (!glfwWindowShouldClose(MainWindow)) {
		//set the background color default
		glClear(GL_COLOR_BUFFER_BIT);

		//get the events in the queue
		//events should be mouse input, keyboard input, controller input...
		glfwPollEvents();		

		//send the framebuffer content to be rendered in the screen
		glfwSwapBuffers(MainWindow);
	}
	

	//finishing GLEW
	glfwTerminate();
	return 0;
}