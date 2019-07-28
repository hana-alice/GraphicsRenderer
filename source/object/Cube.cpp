#include "Cube.h"
#include "GLWrapper.h"
#include <string>
#include <fstream>
#include "CommonFunc.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stbimg/stb_image.h"

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

Cube::Cube(/* args */)
    :m_glWrapper(nullptr)
{
}

Cube::~Cube()
{
    glDetachShader(m_program, m_vertexShader);
	glDetachShader(m_program, m_fragmentShader);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteProgram(m_program);
}

void Cube::init()
{
    std::string srcPath = CommonFunc::getResourceDirectory();
    std::ifstream vsSource,fsSource;

    vsSource.open(srcPath + "/resources/shader/cube.vs");
    fsSource.open(srcPath + "/resources/shader/cube.fs");
    
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
    m_glWrapper = Singleton::getInstance()->getGLWrapper();
    m_vertexShader = m_glWrapper->createShader(GL_VERTEX_SHADER,vs.c_str());
    m_fragmentShader = m_glWrapper->createShader(GL_FRAGMENT_SHADER,fs.c_str());
    m_program = m_glWrapper->createProgram(m_vertexShader,m_fragmentShader);

    float vertices[] = 
    {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

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

    unsigned short indices[] = 
    {
        0,1,3,  3,1,2,  
        2,1,5,  2,5,6,
        5,4,6,  6,4,7,
        7,4,3,  3,4,0,
        3,2,7,  7,2,6,
        0,4,1,  1,4,5
    };

    glGenVertexArrays(1,&m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1,&m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    /*
    glGenBuffers(1,&m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);*/
	GLWrapper::errorCheck();
    glUseProgram(m_program);
    GLint posLoc = glGetAttribLocation(m_program,"aPos");
    glVertexAttribPointer(posLoc,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(posLoc);

    GLint modelLoc = 0, viewLoc = 0, projectionLoc = 0;
    glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
	const glm::mat4* view = Singleton::getInstance()->getViewMat();
	proj = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
	modelLoc = glGetUniformLocation(m_program, "modelMat");
	viewLoc = glGetUniformLocation(m_program, "viewMat");
	projectionLoc = glGetUniformLocation(m_program, "projectionMat");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(*view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &proj[0][0]);
	GLWrapper::errorCheck();

    GLint texLoc = glGetAttribLocation(m_program,"aNorm");
    glVertexAttribPointer(texLoc,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(void*)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(texLoc);
	GLWrapper::errorCheck();

    glUseProgram(m_program);
    m_tex = Singleton::getInstance()->getSkyboxTexture();
	GLuint samplerPos1 = glGetUniformLocation(m_program, "texture1");
	GLWrapper::errorCheck();
	glUniform1i(samplerPos1, 0);
	glActiveTexture(GL_TEXTURE0);
	GLWrapper::errorCheck();

    unsigned int uniformBlockIndex = glGetUniformBlockIndex(m_program,"Matrices");
    glUniformBlockBinding(m_program,uniformBlockIndex,Singleton::getInstance()->getUboId());
	/*
    //unsigned int texture1, texture2;
    glGenTextures(1,&m_tex);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	GLWrapper::errorCheck();
	std::string path = CommonFunc::getResourceDirectory();
    int width, height, channels;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load((path + "/resources/images/jojo0.jpg").c_str(),&width,&height,&channels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        //TODO: exception or debug info here
    }
    stbi_image_free(data);
	GLWrapper::errorCheck();
    glGenTextures(1,&texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texture2);
    glUniform1i(glGetUniformLocation(m_program,"texture2"),1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    data = stbi_load((path + "/resources/images/dio.jpg").c_str(),&width,&height,&channels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        //TODO: exception or debug info here
    }
    stbi_image_free(data);*/
    glBindVertexArray(0);
    glUseProgram(0);
}

void Cube::render()
{
    glViewport(0, 0, 1280, 720);
	glUseProgram(m_program);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

    glBindVertexArray(m_vao);
	GLint viewLoc = glGetUniformLocation(m_program, "viewMat");
	const glm::mat4* view = Singleton::getInstance()->getViewMat();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(*view));

    glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
	GLint projectionLoc = glGetUniformLocation(m_program, "projectionMat");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &proj[0][0]);

    glm::mat4 model = glm::mat4(1.0f);
    GLint modelLoc = glGetUniformLocation(m_program, "modelMat");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glm::vec3 camPos = Singleton::getInstance()->getCameraPosition();
    GLint camPosLoc = glGetUniformLocation(m_program, "camPos");
    glUniform3fv(camPosLoc, 1, glm::value_ptr(camPos));

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Singleton::getInstance()->getSkyboxTexture());
    glDrawArrays(GL_TRIANGLES,0,36);
    GLWrapper::errorCheck();
    
    
    glBindVertexArray(0);
    glUseProgram(0);
}

void Cube::destroy()
{
    delete this;
}