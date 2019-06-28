#include "Widget.h"
#include "Singleton.h"
#include "SampleBox.h"
#include "GLWrapper.h"
#include "Cube.h"

Widget::Widget(/* args */)
    :m_window(nullptr)
{
    m_glWrapper = new GLWrapper;
}

Widget::~Widget()
{
}

void Widget::init()
{
    SampleBox* sampleBox = new SampleBox;
    m_glWrapper->registerInitFunc(sampleBox,&SampleBox::init);
    m_glWrapper->registerRenderFunc(sampleBox,&SampleBox::render);
    m_glWrapper->registerDestroyFunc(sampleBox,&SampleBox::destroy);

    Cube* cube = new Cube;
    m_glWrapper->registerRenderFunc(cube,&Cube::init);
    m_glWrapper->registerRenderFunc(cube,&Cube::render);
    m_glWrapper->registerDestroyFunc(cube,&Cube::destroy);

    m_glWrapper->init();

}

void Widget::setWindow(GLFWwindow* wd)
{
    m_window = wd;
}

void Widget::render()
{
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_glWrapper->render();

#ifdef GLFWAPI
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(m_window);
    glfwPollEvents();
#endif
}