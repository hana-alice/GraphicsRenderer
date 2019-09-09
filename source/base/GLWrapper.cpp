#include "GLWrapper.h"
#include <iostream>
#include "Singleton.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
    initUbo();
    for(InitFunc f : m_initFuncVec)
        f();
}

void GLWrapper::initUbo()
{
    glGenBuffers(1,&m_uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER,m_uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4),NULL,GL_STATIC_DRAW);
	unsigned int ubi = 2;
    Singleton::getInstance()->setUboBlockId(ubi);
    glBindBufferRange(GL_UNIFORM_BUFFER, ubi,m_uboMatrices,0,2*sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLWrapper::render()
{
    preRenderFunc();

    for (RenderFunc f : m_renderFuncVec)
        f();
        
    postRenderFunc();
}

void GLWrapper::preRenderFunc()
{
    glm::mat4 projection = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0/720.0), 0.1f, 100.0f);
    const glm::mat4* view = Singleton::getInstance()->getViewMat();
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(*view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glEnable(GL_MULTISAMPLE);
}

void GLWrapper::postRenderFunc()
{

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

GLuint GLWrapper::createProgram(int vertexShader, int fragmentShader,int geometryShader)
{
    GLuint pgm = glCreateProgram();
    glAttachShader(pgm,vertexShader);
    glAttachShader(pgm,fragmentShader);
    if (geometryShader != -1)
        glAttachShader(pgm,geometryShader);
    
    glLinkProgram(pgm);
    errorCheck();
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryShader != -1)
        glDeleteShader(geometryShader);
    return pgm;
}