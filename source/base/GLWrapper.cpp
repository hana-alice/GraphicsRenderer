#include "GLWrapper.h"
#include <iostream>
#include "Singleton.h"

GLWrapper::GLWrapper()
{
    Singleton::getInstance()->setGLWrapper(this);
}

GLWrapper::~GLWrapper()
{
    for(DestroyFunc f : m_destroyFuncVec)
        f();
}

void GLWrapper::errorCheck()
{
    GLenum errtype = glGetError();
    if (errtype != GL_NO_ERROR)
    {
        std::cout << errtype << std::endl;
    }
}

void GLWrapper::registerInitFunc(InitFunc f)
{
    m_initFuncVec.push_back(f);
}

void GLWrapper::registerRenderFunc(RenderFunc f)
{
    m_renderFuncVec.push_back(f);
}

void GLWrapper::registerDestroyFunc(DestroyFunc f)
{
    m_destroyFuncVec.push_back(f);
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
    errorCheck();
    return shader;
}

GLuint GLWrapper::createProgram(int vertexShader, int fragmentShader)
{
    GLuint pgm = glCreateProgram();
    glAttachShader(pgm,vertexShader);
    glAttachShader(pgm,fragmentShader);
    glLinkProgram(pgm);
    errorCheck();
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return pgm;
}