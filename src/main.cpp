#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Shader.h"
#include "stb_image.h"
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include <GL/glm/gtc/type_ptr.hpp>


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_NAME "LearnOpenGL"


void framebuffer_size_callback(GLFWwindow* window, int width, int height); // Allow to resize the window and the viewport as well, GLFW gonna use this function on its own.
void processInput(GLFWwindow* window); // Process all input from the window
	float alphaAwesomeFace = 0.0f;
	float degree = 0.0f;

	// Guess it's not for now xd
	enum { IDLE, UP, DOWN, LEFT, RIGHT };
	int State;

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
	  // Position  				// Tex Coords
	  0.5f,  0.5f,  0.0f,  1.0f, 1.0f, // Top right
	  0.5f, -0.5f,  0.0f,  1.0f, 0.0f, // Bottom right
	 -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, // Bottom left
	 -0.5f,  0.5f,  0.0f,  0.0f, 1.0f  // Top left
};

unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
};

	// 0. copy our vertices array in a buffer for OpenGL to use
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Tex Coords Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);


// LOAD AND CREATE A TEXTURE
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
	stbi_set_flip_vertically_on_load(true); // Flip the image vertically to have the right view
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Load and generates the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load("../res/tex/container.jpg", &width, &height, &nrChannels, 0);
	if(data)
	{
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	  glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
	  std::cout << "Failed to load texture\n";
	}
	stbi_image_free(data);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load("../res/tex/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    	glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}
	stbi_image_free(data);

	ourShader.Use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	ourShader.SetInt("texture2", 1);




	while (!glfwWindowShouldClose(window)) // Render Loop, check if the user closed the window, by clicking X or else.
	{

		processInput(window);

		// TRANSFORM

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f));


		// RENDER

		//glClearColor(r,g,b,1.0f);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.Use();
		// Pass the transform matrix to the uniform
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		ourShader.SetFloat("awesomefacealpha", alphaAwesomeFace);

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);


		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			alphaAwesomeFace += 0.01f;
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			alphaAwesomeFace -= 0.01f;

		if(alphaAwesomeFace < 0.0f)
			alphaAwesomeFace = 0.0f;
		if(alphaAwesomeFace > 1.0f)
			alphaAwesomeFace = 1.0f;

	}
	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			degree += 1.0f;
		if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			degree -= 1.0f;
		if(degree > 359.0f)
			degree = 0.0f;
		if(degree < 0.0f)
			degree = 359.0f;
	}


}
