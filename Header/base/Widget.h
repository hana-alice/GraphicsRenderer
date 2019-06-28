#pragma once
#include "GLWrapper.h"
#include "GLFW\glfw3.h"

class SampleBox;

class Widget
{
public:
    Widget(/* args */);
    ~Widget();

    void init();

    void render();

    void setWindow(GLFWwindow* wd);//TODO: temporarily add here, should be integrated onto this class later
private:
    GLWrapper*  m_glWrapper;
    GLFWwindow* m_window;
    SampleBox*  m_sampleBox;
};

