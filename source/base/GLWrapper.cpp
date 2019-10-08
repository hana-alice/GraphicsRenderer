#include "GLWrapper.h"
#include <iostream>
#include "Singleton.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "CommonFunc.h"
#include <string>
#include <sstream>
#include <fstream>

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
    initFbo();
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

	errorCheck();
    
}

void GLWrapper::initFbo()
{
    //FBO
    glGenFramebuffers(1,&m_depthFbo);
    glGenTextures(1,&m_depthTexture);
    glBindTexture(GL_TEXTURE_2D,m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,SCR_WIDTH,SCR_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindFramebuffer(GL_FRAMEBUFFER,m_depthFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture,0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER,0);//default framebuffer, if on xOS 0 may not be the right value.
	errorCheck();
    Singleton::getInstance()->setDepthTexture(m_depthTexture);
   
   //shadow program
    std::string srcPath = CommonFunc::getResourceDirectory();
    std::ifstream vsSource,fsSource;

    vsSource.open(srcPath + "/resources/shader/shadowshader.vs");
    fsSource.open(srcPath + "/resources/shader/shadowshader.fs");
    
	std::string bufStr;
	std::string vs, fs;
	while (getline(vsSource, bufStr))
	{
		vs += (bufStr + '\n');
	}
	while (getline(fsSource, bufStr))
	{
		fs += (bufStr + '\n');
	}
    unsigned int vshader = createShader(GL_VERTEX_SHADER,vs.c_str());
    unsigned int fshader = createShader(GL_FRAGMENT_SHADER,fs.c_str());
    unsigned int shadowPgm = createProgram(vshader,fshader);
    unsigned int uniformBlockIndex = glGetUniformBlockIndex(shadowPgm,"Matrices");
    glUniformBlockBinding(shadowPgm,uniformBlockIndex,Singleton::getInstance()->getUboBlockId());
    Singleton::getInstance()->setShadowProgram(shadowPgm);

    //vbo to render texture
    unsigned short verts[] = {
        -1.0,-1.0,0.0,  
        1.0,-1.0,0.0,
        -1.0,1.0,0.0,

        1.0,-1.0,0.0,
        1.0,1.0,0.0,
        -1.0,1.0,0.0    
    };

    GLuint texVbo;
    glGenBuffers(1,&texVbo);
    glBindBuffer(GL_ARRAY_BUFFER,texVbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(verts),verts,GL_STATIC_DRAW);
    Singleton::getInstance()->setTexVBO(texVbo);

    //program to render texture
	std::ifstream texvsSource, texfsSource;
	texvsSource.open(srcPath + "/resources/shader/texture.vs");
	texfsSource.open(srcPath + "/resources/shader/texture.fs");
    
	std::string texvs, texfs;
	while (getline(texvsSource, bufStr))
	{
		texvs += (bufStr + '\n');
	}
	while (getline(texfsSource, bufStr))
	{
		texfs += (bufStr + '\n');
	}
    vshader = createShader(GL_VERTEX_SHADER,texvs.c_str());
    fshader = createShader(GL_FRAGMENT_SHADER,texfs.c_str());
    unsigned int texPgm = createProgram(vshader,fshader);
    Singleton::getInstance()->setTextureProgram(texPgm);
	errorCheck();
}

void GLWrapper::render()
{

#pragma region shadow
    Singleton::getInstance()->setRenderTarget(SHADOW);
    glViewport(0,0,SCR_WIDTH, SCR_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER,m_depthFbo);
    glClear(GL_DEPTH_BUFFER_BIT);
	GLWrapper::errorCheck();
    const glm::vec3 pos = Singleton::getInstance()->getParalellLightPos();
    glm::mat4 viewMat = glm::lookAt(pos,glm::vec3(0.0f),glm::vec3(1.0f));
    GLfloat near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	GLWrapper::errorCheck();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMat));
	GLWrapper::errorCheck();
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(lightProjection));
	GLWrapper::errorCheck();
    for (RenderFunc f : m_renderFuncVec)
        f();
	GLuint texPgm = Singleton::getInstance()->getTextureProgram();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(texPgm);
	glEnable(GL_BLEND);
	glBindBuffer(GL_ARRAY_BUFFER, Singleton::getInstance()->getTexVBO());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getDepthTexture());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glDisable(GL_BLEND);
	GLWrapper::errorCheck();
#pragma endregion
Singleton::getInstance()->setRenderTarget(SCENE);
	GLWrapper::errorCheck();
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    preRenderFunc();
	GLWrapper::errorCheck();
    for (RenderFunc f : m_renderFuncVec)
        //f();
		GLWrapper::errorCheck();
    postRenderFunc();
	GLWrapper::errorCheck();
}

void GLWrapper::preRenderFunc()
{
    //Uniform block
    glm::mat4 projection = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0/720.0), 0.1f, 100.0f);
    const glm::mat4* view = Singleton::getInstance()->getViewMat();
	GLWrapper::errorCheck();
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	GLWrapper::errorCheck();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(*view));
	GLWrapper::errorCheck();
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	GLWrapper::errorCheck();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
	GLWrapper::errorCheck();
	glEnable(GL_MULTISAMPLE);
	GLWrapper::errorCheck();
    glViewport(0,0,SCR_WIDTH, SCR_HEIGHT);
	GLWrapper::errorCheck();
    glClear(GL_DEPTH_BUFFER_BIT);
	GLWrapper::errorCheck();
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