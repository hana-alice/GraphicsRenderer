#include "plane.h"
#include "stbimg/stb_image.h"
#include "CommonFunc.h"
#include "GLWrapper.h"
#include <vector>
#include <string>
#include <fstream>
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Singleton.h"

using namespace std;

Plane::Plane()
{

}

Plane::~Plane()
{
    glDetachShader(m_program, m_vertexShader);
	glDetachShader(m_program, m_fragmentShader);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteProgram(m_program);
}

void Plane::init()
{
    float vertices[] = {


        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    };
    std::string srcPath = CommonFunc::getResourceDirectory();
    std::ifstream vsSource,fsSource;

    vsSource.open(srcPath + "/resources/shader/plane.vs");
    fsSource.open(srcPath + "/resources/shader/plane.fs");
    
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
	m_vertexShader = m_glWrapper->createShader(GL_VERTEX_SHADER, vs.c_str());
	m_fragmentShader = m_glWrapper->createShader(GL_FRAGMENT_SHADER, fs.c_str());
	m_program = m_glWrapper->createProgram(m_vertexShader, m_fragmentShader);
	GLWrapper::errorCheck();
    glGenVertexArrays(1,&m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1,&m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_STATIC_DRAW);
    
	GLWrapper::errorCheck();
    glUseProgram(m_program);
    GLint posLoc = glGetAttribLocation(m_program,"aPos");
    glVertexAttribPointer(posLoc,3,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(posLoc);

    GLWrapper::errorCheck();
    GLint texLoc = glGetAttribLocation(m_program,"aTex");
    glVertexAttribPointer(texLoc,2,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)(6*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(texLoc);

    unsigned int uniformBlockIndex = glGetUniformBlockIndex(m_program,"Matrices");
    glUniformBlockBinding(m_program,uniformBlockIndex,Singleton::getInstance()->getUboBlockId());

	glm::mat4 modelMat(1.0);
	modelMat = glm::translate(modelMat, glm::vec3(0.0, 0.5, 0.0));
	modelMat = glm::scale(modelMat, glm::vec3(20.0, 1.0, 20.0));
	GLint modelLoc = glGetUniformLocation(m_program, "modelMat");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

	string path = CommonFunc::getResourceDirectory();
    string imgPath = path + "/resources/images/floor.jpeg";

    glGenTextures(1,&m_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_tex);
    int width, height, channels;
    
    unsigned char* data  = stbi_load(imgPath.c_str(),&width,&height,&channels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        GLWrapper::errorCheck();
        stbi_image_free(data);
    } 
    else
    {
        /* exception */
        stbi_image_free(data);
    }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	GLWrapper::errorCheck();
    
    GLuint texSamplerLoc = glGetUniformLocation(m_program,"plane");
    glUniform1i(texSamplerLoc,0);

    glBindTexture(GL_TEXTURE_2D,0);
    glUseProgram(0);
    glBindVertexArray(0);
	GLWrapper::errorCheck();

}

void Plane::render()
{
    if (Singleton::getInstance()->getRenderTarget() == SCENE)
    {
        glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glUseProgram(m_program);
        GLWrapper::errorCheck();
        glBindVertexArray(m_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_tex);
        GLWrapper::errorCheck();
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D,Singleton::getInstance()->getDepthTexture());
        //GLWrapper::errorCheck();
		const glm::vec3 pos = Singleton::getInstance()->getParalellLightPos();
		//const glm::vec3 pos = Singleton::getInstance()->getCameraPosition();
		glm::mat4 viewMat = glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(1.0f));
		GLfloat near_plane = 1.0f, far_plane = 100.0f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		GLint modelLoc = glGetUniformLocation(m_program, "lightSpaceMatrix");
		glm::mat4 vpMat = lightProjection * viewMat;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(vpMat));
        GLWrapper::errorCheck();
        glDrawArrays(GL_TRIANGLES,0,6);
        GLWrapper::errorCheck();

        glBindTexture(GL_TEXTURE_2D,0);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(0);
        glUseProgram(0);
    }
    else
    {
        #if 0
        GLWrapper::errorCheck();
        GLuint shadowPgm = Singleton::getInstance()->getShadowProgram();
        glUseProgram(shadowPgm);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
		GLWrapper::errorCheck();
        glBindVertexArray(m_shadowVao);
		GLWrapper::errorCheck();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, -7.0, 0.0));
	    model = glm::scale(model, glm::vec3(8.0, 1.0, 8.0));
        GLint modelLoc = glGetUniformLocation(shadowPgm, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,6);
        glBindVertexArray(0);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glUseProgram(0);
		GLWrapper::errorCheck();
        #endif
    }
    
    
}

void Plane::destroy()
{
    delete this;
}