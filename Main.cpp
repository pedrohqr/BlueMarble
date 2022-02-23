#include <iostream>
#include <cassert>
#include <array>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

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

	//defining a triangle with normalized cordinates
	std::array<glm::vec3, 3> Triangle{
		glm::vec3{ -1.0f, -1.0f, 0.0f},
		glm::vec3{  0.0f,  1.0f, 0.0f},
		glm::vec3{  1.0f, -1.0f, 0.0f}
	};

	//Model Matrix
	glm::mat4 ModelMatrix = glm::identity<glm::mat4>();

	//View Matrix
	glm::vec3 Eye = { 10, 10, 10 };
	glm::vec3 Center = { 0, 0, 0 };
	glm::vec3 Up = { 0, 1, 0 };
	glm::mat4 ViewMatrix = glm::lookAt(Eye, Center, Up);

	//Projection Matrix
	constexpr float FOV = glm::radians(45.0f);
	const float AspectRatio = Width / Height;
	const float Near = 0.001f;
	const float Far = 1000.0f;
	glm::mat4 ProjectionMatrix = glm::perspective(FOV, AspectRatio, Near, Far);

	//Model View Projection
	glm::mat4 ModelViewProjection = ProjectionMatrix * ViewMatrix * ModelMatrix;

	//apply ModelViewProjection in vertices of triangle
	for (glm::vec3& Vertex : Triangle) {
		glm::vec4 ProjectedVertex = ModelViewProjection * glm::vec4{ Vertex, 1.0f };
		ProjectedVertex /= ProjectedVertex.w;
		Vertex = ProjectedVertex;
	};

	//now, we copy triangle vertices to GPU buffer
	GLuint VertexBuffer;	//GL unsigned integer to store the position in the buffer

	//OpenGl generates the buffer id of VertexBuffer
	glGenBuffers(1, &VertexBuffer);

	//assign VertexBuffer to copy the triangle data 
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	//copy data to video memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle.data(), GL_STATIC_DRAW);

	//draw :)
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//assign background color
	glClearColor(0.0f, 0.1f, 0.0f, 1.0f);

	//events queue
	while (!glfwWindowShouldClose(MainWindow)) {
		//set the background color default
		glClear(GL_COLOR_BUFFER_BIT);

		glEnableVertexAttribArray(0);

		//assign the VertexBuffer active to OpenGL
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

		//says to OpenGL where is the vertices into VertexBuffer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		//finally, draw
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//disable the currently state of Triangle to can draw something
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);

		

		//get the events in the queue
		//events should be mouse input, keyboard input, controller input...
		glfwPollEvents();		

		//send the framebuffer content to be rendered in the screen
		glfwSwapBuffers(MainWindow);
		
	}

	//free Triangle buffer
	glDeleteBuffers(1, &VertexBuffer);
	

	//finishing GLEW
	glfwTerminate();
	return 0;
}