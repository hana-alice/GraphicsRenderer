#include "GLWrapper.h"
#include <iostream>

GLWrapper::GLWrapper()
{

}

GLWrapper::~GLWrapper()
{

}

bool GLWrapper::registerInitFunc(InitFunc f)
{
    if(std::find(m_initFuncVec.begin(),m_initFuncVec.end(),f) != m_initFuncVec.end())
        m_initFuncVec.push_back(f);
    else
        return false;
    return true;
}

bool GLWrapper::registerRenderFunc(RenderFunc f)
{
    if(std::find(m_renderFuncVec.begin(),m_renderFuncVec.end(),f) != m_renderFuncVec.end())
        m_renderFuncVec.push_back(f);
    else
        return false;
    return true;
}

void GLWrapper::init()
{
    for(InitFunc f : m_initFuncVec)
        f();
}

void GLWrapper::render()
{
    for (RenderFunc f : m_renderFuncVec)
        f();
}

GLuint GLWrapper::createShader(int type, const char* shaderSrc)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader,1,&shaderSrc,NULL);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(shader,1024,NULL,infoLog);
        std::cout << "compile Err:\n" << infoLog;
    }
    
    return 0;
}

GLuint GLWrapper::createProgram(int vertexShader, int fragmentShader)
{
    return 0;
}