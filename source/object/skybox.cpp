#include "skybox.h"
#include "stbimg/stb_image.h"
#include "CommonFunc.h"
#include "GLWrapper.h"
#include <vector>
#include <string>
#include <fstream>
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

SkyBox::SkyBox()
{

}

SkyBox::~SkyBox()
{
    glDetachShader(m_program, m_vertexShader);
	glDetachShader(m_program, m_fragmentShader);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteProgram(m_program);
}

void SkyBox::init()
{
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    std::string srcPath = CommonFunc::getResourceDirectory();
    std::ifstream vsSource,fsSource;

    vsSource.open(srcPath + "/resources/shader/skybox.vs");
    fsSource.open(srcPath + "/resources/shader/skybox.fs");
    
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
    glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),skyboxVertices,GL_STATIC_DRAW);
    
	GLWrapper::errorCheck();
    glUseProgram(m_program);
    GLint posLoc = glGetAttribLocation(m_program,"aPos");
    glVertexAttribPointer(posLoc,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(posLoc);

    std::vector<string> pathVec;
    string path = CommonFunc::getResourceDirectory();
    pathVec.push_back(path + "/resources/images/skybox/right.jpg");
    pathVec.push_back(path + "/resources/images/skybox/left.jpg");
    pathVec.push_back(path + "/resources/images/skybox/top.jpg");
    pathVec.push_back(path + "/resources/images/skybox/bottom.jpg");
    pathVec.push_back(path + "/resources/images/skybox/front.jpg");
    pathVec.push_back(path + "/resources/images/skybox/back.jpg");

    glGenTextures(1,&m_tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP,m_tex);
    int width, height, channels;
    for (size_t i = 0; i < pathVec.size(); i++)
    {
        unsigned char* data  = stbi_load(pathVec[i].c_str(),&width,&height,&channels,0);
        if(data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
			GLWrapper::errorCheck();
            stbi_image_free(data);
        } 
        else
        {
            /* exception */
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    Singleton::getInstance()->setSkyboxTexture(m_tex);
	GLWrapper::errorCheck();
    glUseProgram(m_program);
    GLuint texSamplerLoc = glGetUniformLocation(m_program,"skyBox");
    glUniform1i(texSamplerLoc,0);
    glUseProgram(0);
	GLWrapper::errorCheck();
}

void SkyBox::render()
{
    glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
    glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
    glUseProgram(m_program);

    glBindVertexArray(m_vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_tex);
	glActiveTexture(GL_TEXTURE0);

	GLint viewLoc = glGetUniformLocation(m_program, "viewMat");
	const glm::mat4* view = Singleton::getInstance()->getViewMat();
	//const glm::vec3 campos = Singleton::getInstance()->getCameraPosition();
	glm::mat4 originView = glm::lookAt(glm::vec3(0, 0, 0), Singleton::getInstance()->getCameraFront(), glm::vec3(0.0,1.0,0.0));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(originView));

	glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
	GLint projectionLoc = glGetUniformLocation(m_program, "projectionMat");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &proj[0][0]);

    glDrawArrays(GL_TRIANGLES,0,36);
	GLWrapper::errorCheck();
	glBindTexture(GL_TEXTURE_CUBE_MAP,0);
	glUseProgram(0);
    glDepthMask(GL_TRUE);
}

void SkyBox::destroy()
{
    delete this;
}