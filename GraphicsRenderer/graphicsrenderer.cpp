#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimg/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camDirection = glm::vec3(camTarget - camPos);//reverse for positive
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camRight = glm::normalize(glm::cross(up, camDirection));
//glm::vec3 camUp = glm::normalize(glm::cross(camDirection, camRight));
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camStraitFront = glm::vec3(0.0f, 0.0f, -1.0f);

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glm::vec3 coral(0.33f, 0.42f, 0.18f);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 result = lightColor * coral;


	Shader ourShader("vertexShader.vs", "fragShader.fs");

	unsigned int objVBO, objVAO;
	glGenVertexArrays(1, &objVAO);
	glBindVertexArray(objVAO);
	glGenBuffers(1, &objVBO);
	glBindBuffer(GL_ARRAY_BUFFER, objVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLint objPosLoc, objColorLoc;
	objPosLoc = glGetAttribLocation(ourShader.ID, "aPos");
	objColorLoc = glGetUniformLocation(ourShader.ID, "inColor");
	glEnableVertexAttribArray(objPosLoc);

	glm::mat4 modelMat(1.0f), projMat(1.0f), viewMat(1.0f);
	modelMat = glm::rotate(modelMat, (float)-60, glm::vec3(1.0f, 0.0f, 0.0f));
	projMat = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
	viewMat = glm::lookAt(camPos, camPos + camStraitFront, camUp);

	

	ourShader.use();


	while (!glfwWindowShouldClose(window))
	{

		processInput(window);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		ourShader.use();
		glBindVertexArray(objVAO);
		glBindBuffer(GL_ARRAY_BUFFER, objVBO);

		GLint modelLoc, viewLoc, projLoc;
		modelLoc = glGetUniformLocation(ourShader.ID, "model");
		viewLoc = glGetUniformLocation(ourShader.ID, "view");
		projLoc = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMat));

		glVertexAttribPointer(objPosLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		glUniform4f(objColorLoc, 1.0f, 1.0f, 0.0f, 1.0f);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &objVAO);
	glDeleteBuffers(1, &objVBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
