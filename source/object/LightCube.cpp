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

#pragma region vertices
float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

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
#pragma endregion

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

    glGenVertexArrays(1,&m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1,&m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    glUseProgram(m_program);
    GLint posLoc = glGetAttribLocation(m_program,"aPos");
    glVertexAttribPointer(posLoc,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(posLoc);

    GLint modelLoc = 0, viewLoc = 0, projectionLoc = 0, lightColorLoc = 0, objColorLoc = 0, ltPosLoc = 0;
    glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
	GLWrapper::errorCheck();
	const glm::mat4 *view = Singleton::getInstance()->getViewMat();
	proj = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
	modelLoc = glGetUniformLocation(m_program, "model");
	viewLoc = glGetUniformLocation(m_program, "view");
	projectionLoc = glGetUniformLocation(m_program, "projection");
    lightColorLoc = glGetUniformLocation(m_program, "lightColor");
    objColorLoc   = glGetUniformLocation(m_program, "objectColor");
	GLWrapper::errorCheck();
    glm::vec3 lightPos = glm::vec3(1.2,1.0,2.0);
    if(m_type == LIGHT)
    {
        model = glm::translate(model,lightPos);
        model = glm::scale(model,glm::vec3(0.2));
    }
    else 
    {
		GLWrapper::errorCheck();
		GLint normLoc = glGetAttribLocation(m_program, "aNormal");
		glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(normLoc);
        ltPosLoc = glGetUniformLocation(m_program,"lightPos");
        glUniform3fv(ltPosLoc,1,glm::value_ptr(lightPos));
    }
	GLWrapper::errorCheck();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(*view));
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
	const glm::mat4* view = Singleton::getInstance()->getViewMat();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(*view));

    GLint viewPosLoc = glGetUniformLocation(m_program,"viewPos");
    glUniform3fv(viewPosLoc,1,glm::value_ptr(Singleton::getInstance()->getCameraPosition()));

    glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
	GLint projectionLoc = glGetUniformLocation(m_program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &proj[0][0]);

    glm::vec3 mAmbient = glm::vec3(1.0f,0.5f,0.31f);
	GLint mAmbientLoc = glGetUniformLocation(m_program, "material.ambient");
    glUniform3fv(mAmbientLoc,1,glm::value_ptr(mAmbient));

    glm::vec3 mDiffuse = glm::vec3(1.0f,0.5f,0.31f);
	GLint mDiffuseLoc = glGetUniformLocation(m_program, "material.diffuse");
    glUniform3fv(mDiffuseLoc,1,glm::value_ptr(mDiffuse));

    glm::vec3 mSpecular = glm::vec3(0.5f,0.5f,0.5f);
	GLint mSpecularLoc = glGetUniformLocation(m_program, "material.specular");
    glUniform3fv(mSpecularLoc,1,glm::value_ptr(mSpecular));

	float shininess = 32;
	GLint shininessLoc = glGetUniformLocation(m_program, "material.shininess");
	glUniform1f(shininessLoc, shininess);

	glm::vec3 lAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
	GLint lAmbientLoc = glGetUniformLocation(m_program, "light.ambient");
	glUniform3fv(lAmbientLoc, 1, glm::value_ptr(lAmbient));

	glm::vec3 lDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	GLint lDiffuseLoc = glGetUniformLocation(m_program, "light.diffuse");
	glUniform3fv(lDiffuseLoc, 1, glm::value_ptr(lDiffuse));

	glm::vec3 lSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	GLint lSpecularLoc = glGetUniformLocation(m_program, "light.specular");
	glUniform3fv(lSpecularLoc, 1, glm::value_ptr(lSpecular));
    GLWrapper::errorCheck();
    glDrawArrays(GL_TRIANGLES,0,36);
      
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

