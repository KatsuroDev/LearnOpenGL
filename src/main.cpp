#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_NAME "LearnOpenGL"


void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Allow to resize the window and the viewport as well, GLFW gonna use this function on its own.
void processInput(GLFWwindow* window); // Process all input from the window


int main()
{

	// Init GLFW and telling we are gonna use the opengl 3.3 version in the core-profile
	glfwInit();


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Only to use on MACOS
#endif


	// Create a GLFW window, with the dimensions, the name and other things.

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "[ERROR] : Failed to create GLFW window\n"; // Testing if we got the window right
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // Telling GLFW what current context we'll use.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // But we do have to tell glfw which function is the sizecallback one.

	if (glewInit() != GLEW_OK) // Init GLEW
		return -1;


Shader ourShader("../res/shaders/shader.vs", "../res/shaders/shader.fs");

float vertices[] = {
	  // Position           // Colors
	  0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f,
	 -0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,
	  0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f
};


	// 0. copy our vertices array in a buffer for OpenGL to use
	unsigned int VBO, VAO/*, EBO*/;
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

  // Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window)) // Render Loop, check if the user closed the window, by clicking X or else.
	{

		processInput(window);

		//RENDER

		//glClearColor(r,g,b,1.0f);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window); // Swap the buffers every iterations
		glfwPollEvents(); // Check events
	}

	//glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}
