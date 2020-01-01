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
    initGBuffer();
    for(InitFunc f : m_initFuncVec)
        f();
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void GLWrapper::initGBuffer()
{
    glGenFramebuffers(1,&m_gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    GLuint position, normal, albedoSpec;
    Singleton::getInstance()->setGBuffer(m_gBuffer);

    glGenTextures(1, &position);
    glBindTexture(GL_TEXTURE_2D, position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH,SCR_HEIGHT,0,GL_RGB,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position,0);
    Singleton::getInstance()->setGBufferPosTexture(position);

    glGenTextures(1, &normal);
    glBindTexture(GL_TEXTURE_2D, normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH,SCR_HEIGHT,0,GL_RGB,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal,0);
    Singleton::getInstance()->setGBufferNormTexture(normal);

    glGenTextures(1, &albedoSpec);
    glBindTexture(GL_TEXTURE_2D, albedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedoSpec,0);
    Singleton::getInstance()->setGBufferAlbedoSpec(albedoSpec);

    GLuint attacments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attacments);

    //render target --- depth
    glGenRenderbuffers(1,&m_depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER,m_depthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "framebuffer not complete." << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Singleton::getInstance()->setDepthRBO(m_depthRbo);
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
	GLWrapper::errorCheck();
    //FBO
    glGenFramebuffers(1,&m_depthFbo);
    glGenTextures(1,&m_depthTexture);
    glBindTexture(GL_TEXTURE_2D,m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,SCR_WIDTH,SCR_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindFramebuffer(GL_FRAMEBUFFER,m_depthFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture,0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER,0);//default framebuffer, if on xOS 0 may not be the right value.
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
    glGenVertexArrays(1,&m_texVao);
	GLWrapper::errorCheck();
    glBindVertexArray(m_texVao);
	GLWrapper::errorCheck();
    float verts[] = {
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
	GLuint pLoc = glGetAttribLocation(texPgm, "aPos");
	glVertexAttribPointer(pLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(pLoc);

	glUseProgram(texPgm);
	glActiveTexture(GL_TEXTURE0);
	GLWrapper::errorCheck();
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	
    GLuint texSampLoc = glGetUniformLocation(texPgm,"texCanvas");
	GLWrapper::errorCheck();
    glUniform1i(texSampLoc,0);
	glUseProgram(0);
	glBindVertexArray(0);
    Singleton::getInstance()->setTextureProgram(texPgm);
	errorCheck();
}

void GLWrapper::render()
{
#if 0
#pragma region shadow
    Singleton::getInstance()->setRenderTarget(SHADOW);
    glViewport(0,0,SCR_WIDTH, SCR_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER,m_depthFbo);
    glClear(GL_DEPTH_BUFFER_BIT);
	GLWrapper::errorCheck();
    const glm::vec3 pos = Singleton::getInstance()->getParalellLightPos();
    glm::mat4 viewMat = glm::lookAt(pos,glm::vec3(0.0f),glm::vec3(1.0f));
    GLfloat near_plane = 1.0f, far_plane = 100.0f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	//glm::mat4 lightProjection = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMat));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(lightProjection));

    for (RenderFunc f : m_renderFuncVec)
        f();


	GLWrapper::errorCheck();
	GLuint texPgm = Singleton::getInstance()->getTextureProgram();
	GLWrapper::errorCheck();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(texPgm);
	glEnable(GL_BLEND);
	glViewport(0, 0, 1280, 720);
	glBindVertexArray(m_texVao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	//GLWrapper::errorCheck();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);
	GLWrapper::errorCheck();
#endif
#pragma endregion
#if 1
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Singleton::getInstance()->setRenderTarget(SCENE);
	GLWrapper::errorCheck();
    preRenderFunc();
	GLWrapper::errorCheck();
    for (RenderFunc f : m_renderFuncVec)
        f();
	GLWrapper::errorCheck();
    postRenderFunc();
	GLWrapper::errorCheck();
#endif
}

void GLWrapper::preRenderFunc()
{
    //Uniform block
    glm::mat4 projection = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0/720.0), 0.1f, 100.0f);
    const glm::mat4* view = Singleton::getInstance()->getViewMat();
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(*view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glEnable(GL_MULTISAMPLE);
    glViewport(0,0,SCR_WIDTH, SCR_HEIGHT);
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