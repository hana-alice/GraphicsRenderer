#include "Widget.h"
#include "Singleton.h"
#include "SampleBox.h"
#include "GLWrapper.h"
#include "Cube.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* window = nullptr;
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
static glm::vec3 camPos = Singleton::getInstance()->getCameraPosition();
static glm::vec3 camFront = Singleton::getInstance()->getCameraFront();
static glm::vec3 camUp = Singleton::getInstance()->getCameraUp();

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* wd);

Widget::Widget(/* args */)
    :m_width(1280),m_height(720),
    m_initialised(false)
{
    m_glWrapper = new GLWrapper;
}

Widget::~Widget()
{
}

void Widget::init()
{
    if(m_initialised)
        return;
	initContext();
	initObject();
    m_initialised = true;
}

void Widget::render()
{
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_glWrapper->render();

#ifdef GLFWAPI
    glfwSwapBuffers(window);
    glfwPollEvents();
#endif
}

void Widget::show()
{
	init();
	GLWrapper::errorCheck();
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

        //keyboard and mouse
		processInput(window);

		// render
		render();
		
	}

	glfwTerminate();
}

void Widget::initContext()
{
	// glfw init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	window = glfwCreateWindow(m_width, m_height, "GraphicsRender", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
}

void Widget::initObject()
{
	//init object
	SampleBox* sampleBox = new SampleBox;
	m_glWrapper->registerInitFunc(sampleBox, &SampleBox::init);
	//m_glWrapper->registerRenderFunc(sampleBox,&SampleBox::render);
	m_glWrapper->registerDestroyFunc(sampleBox, &SampleBox::destroy);

	Cube* cube = new Cube;
	m_glWrapper->registerInitFunc(cube, &Cube::init);
	m_glWrapper->registerRenderFunc(cube, &Cube::render);
	m_glWrapper->registerDestroyFunc(cube, &Cube::destroy);

	m_glWrapper->init();
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *wd)
{
    //no need to check if window is valid cuz this func exec after glfwWindowShouldClose(window)
    float camSpeed = 2.5 * deltaTime;
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
        camPos += camSpeed * camFront;
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
        camPos -= camSpeed * camFront;
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
        camPos -= glm::normalize(glm::cross(camFront,camUp)) * camSpeed;//front first, up later, cross get a vector towards right, hence subtract
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
        camPos += glm::normalize(glm::cross(camFront,camUp)) * camSpeed;
    Singleton::getInstance()->setCameraPosition(camPos);
}