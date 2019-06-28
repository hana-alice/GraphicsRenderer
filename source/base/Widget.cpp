#include "Widget.h"
#include "Singleton.h"
#include "SampleBox.h"
#include "GLWrapper.h"

Widget::Widget(/* args */)
    :m_sampleBox(nullptr),
    m_window(nullptr)
{
    m_glWrapper = new GLWrapper;
}

Widget::~Widget()
{
}

void Widget::init()
{
    m_sampleBox = new SampleBox;
    m_glWrapper->registerInitFunc(m_sampleBox,&SampleBox::init);
	GLWrapper::errorCheck();
    m_glWrapper->registerRenderFunc(m_sampleBox,&SampleBox::render);
	GLWrapper::errorCheck();
    m_glWrapper->init();
	GLWrapper::errorCheck();
}

void Widget::setWindow(GLFWwindow* wd)
{
    m_window = wd;
}

void Widget::render()
{
    glClearColor(0.0f, 0.7f, 0.8f, 1.0f);
	GLWrapper::errorCheck();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GLWrapper::errorCheck();

    m_glWrapper->render();

#ifdef GLFWAPI
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(m_window);
    glfwPollEvents();
#endif
}