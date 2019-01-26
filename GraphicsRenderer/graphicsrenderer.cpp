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
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
float vertices[] =
{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camDirection = glm::vec3(camTarget - camPos);//reverse for positive
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camRight = glm::normalize(glm::cross(up, camDirection));
//glm::vec3 camUp = glm::normalize(glm::cross(camDirection, camRight));
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camStraitFront = glm::vec3(0.0f, 0.0f, -1.0f);

float lastX = 400, lastY = 300;

bool isFirstIn = true;

float yaw = 0.0, pitch = 0.0, roll = 0.0;
float fov = 45.0;
// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

glm::vec3 lightColor(0.0f, 1.0f, 0.0f);
glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
glm::vec3 result = lightColor * toyColor; // = (0.0f, 0.5f, 0.0f);
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	Shader objShader("vertexShader.vs", "fragShader.fs");

	unsigned int objVBO, objVAO, objEBO;
	glGenVertexArrays(1, &objVAO);
	glGenBuffers(1, &objVBO);
	glGenBuffers(1, &objEBO);

	glBindVertexArray(objVAO);

	glBindBuffer(GL_ARRAY_BUFFER, objVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLint posLoc = glGetAttribLocation(objShader.ID, "aPos");
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(posLoc);
	GLint normLoc = glGetAttribLocation(objShader.ID, "aNormal");
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(normLoc);
	
	Shader lightShader("lightVertexShader.vs", "lightFragShader.fs");
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, objVBO);
	posLoc = glGetAttribLocation(lightShader.ID, "aPos");
	glVertexAttribPointer(posLoc, 3, GL_FLOAT,GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(posLoc);



	while (!glfwWindowShouldClose(window))
	{

		processInput(window);

		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 modelMat(1.0);
		
		glm::mat4 projMat(1.0);
		projMat = glm::perspective(glm::radians(fov), (float)(SCR_WIDTH / SCR_HEIGHT),0.1f, 100.0f);
		glm::mat4 viewMat(1.0);
		viewMat = glm::lookAt(camPos, camPos + camStraitFront, camUp);

		objShader.use();
		unsigned int loc = glGetUniformLocation(objShader.ID, "model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(modelMat));
		loc = glGetUniformLocation(objShader.ID, "view");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewMat));
		loc = glGetUniformLocation(objShader.ID, "projection");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projMat));
		
		float fragcolor[] = { 1.0f,1.0f,0.0f,1.0f };
		loc = glGetUniformLocation(objShader.ID, "lightColor");
		glUniform3f(loc, 1.0f, 1.0f, 1.0f);
		loc = glGetUniformLocation(objShader.ID, "objectColor");
		glUniform3f(loc, 1.0f, 0.5f, 0.31f);
		loc = glGetUniformLocation(objShader.ID, "lightPos");
		glUniform3fv(loc,1, glm::value_ptr(lightPos));
		loc = glGetUniformLocation(objShader.ID, "viewPos");
		glUniform3fv(loc, 1, glm::value_ptr(camPos));

		glBindVertexArray(objVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightShader.use();
		glBindVertexArray(lightVAO);
		modelMat = glm::translate(modelMat, lightPos);
		modelMat = glm::scale(modelMat, glm::vec3(0.2f));
		loc = glGetUniformLocation(lightShader.ID, "model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(modelMat));
		loc = glGetUniformLocation(lightShader.ID, "view");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewMat));
		loc = glGetUniformLocation(lightShader.ID, "projection");
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projMat));
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &objVAO);
	glDeleteBuffers(1, &objVBO);
	glDeleteBuffers(1, &objEBO);

	glfwTerminate();
	return 0;
}

float deltaTime = 0;
float lastTime = 0;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float curTime = glfwGetTime();
	deltaTime = curTime - lastTime;
	lastTime = curTime;
	float camspeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camPos += camspeed * glm::normalize(camStraitFront);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camPos -= camspeed * glm::normalize(camStraitFront);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camPos -= camspeed * glm::normalize(glm::cross(camStraitFront, camUp));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camPos += camspeed * glm::normalize(glm::cross(camStraitFront, camUp));
	}

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (isFirstIn)
	{
		lastX = xpos;
		lastY = ypos;
		isFirstIn = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // different coord

	lastX = xpos;
	lastY = ypos;

	float sendsitivity = 0.05f;
	xoffset *= sendsitivity;
	yoffset *= sendsitivity;

	yaw += xoffset;
	pitch += yoffset;
	
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	camStraitFront = glm::normalize(front);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}