#include "lightCube.h"
#include "CommonFunc.h"
#include "Singleton.h"
#include "GLWrapper.h"
#include <string>
#include <fstream>
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <sstream>

LightCube::LightCube(CUBE_TYPE tp):m_type(tp)
{
}

LightCube::~LightCube()
{
}

void LightCube::init()
{
    std::string srcPath = CommonFunc::getResourceDirectory();
    std::ifstream vsSource,fsSource;

    if(m_type == OBJECT)
    {
        vsSource.open(srcPath + "/resources/shader/object.vs");
	    fsSource.open(srcPath + "/resources/shader/object.fs");
    }
    else if(m_type == LIGHT)
    {
        vsSource.open(srcPath + "/resources/shader/light.vs");
	    fsSource.open(srcPath + "/resources/shader/light.fs");
    }
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
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f
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
    
    glGenBuffers(1,&m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
	GLWrapper::errorCheck();
    glUseProgram(m_program);
    GLint posLoc = glGetAttribLocation(m_program,"aPos");
    glVertexAttribPointer(posLoc,3,GL_FLOAT,GL_FALSE,5*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(posLoc);

    GLint modelLoc = 0, viewLoc = 0, projectionLoc = 0, lightColorLoc = 0, objColorLoc = 0;
    glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
    if(m_type == LIGHT)
    {
        model = glm::translate(model,glm::vec3(1.2,1.0,2.0));
        model = glm::scale(model,glm::vec3(0.2));
    }
	view = Singleton::getInstance()->getViewMat();
	proj = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
	modelLoc = glGetUniformLocation(m_program, "model");
	viewLoc = glGetUniformLocation(m_program, "view");
	projectionLoc = glGetUniformLocation(m_program, "projection");
    lightColorLoc = glGetUniformLocation(m_program, "lightColor");
    objColorLoc   = glGetUniformLocation(m_program, "objectColor");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &proj[0][0]);
    glUniform3f(lightColorLoc,1.0f, 1.0f, 1.0f);
    glUniform3f(objColorLoc,1.0f, 0.5f, 0.31f);
    glUseProgram(0);
    glBindVertexArray(0);
	GLWrapper::errorCheck();
}

void LightCube::render()
{
    glViewport(0, 0, 1280, 720);
	glUseProgram(m_program);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

    glBindVertexArray(m_vao);
	GLint viewLoc = glGetUniformLocation(m_program, "view");
	glm::mat4 view = glm::mat4(1.0f);
	view = Singleton::getInstance()->getViewMat();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
	GLint projectionLoc = glGetUniformLocation(m_program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &proj[0][0]);

    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,NULL);
      
    glBindVertexArray(0);
    glUseProgram(0);
}

void LightCube::destroy()
{
    glDetachShader(m_program, m_vertexShader);
	glDetachShader(m_program, m_fragmentShader);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteProgram(m_program);
}

