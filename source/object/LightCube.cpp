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
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stbimg/stb_image.h"

#pragma region vertices
static float vertices[] = {
         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

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
#pragma endregion

LightCube::LightCube()
{
}

LightCube::~LightCube()
{
}

void LightCube::init()
{
    std::string srcPath = CommonFunc::getResourceDirectory();
    std::ifstream vsSource,fsSource;
    
    vsSource.open(srcPath + "/resources/shader/object.vs");
    fsSource.open(srcPath + "/resources/shader/object.fs");
    
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

    unsigned int uniformBlockIndex = glGetUniformBlockIndex(m_program,"Matrices");
    glUniformBlockBinding(m_program,uniformBlockIndex,Singleton::getInstance()->getUboBlockId());
    GLWrapper::errorCheck();
    GLint posLoc = glGetAttribLocation(m_program,"aPos");
    glVertexAttribPointer(posLoc,3,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(posLoc);

    GLint modelLoc = 0, viewLoc = 0, projectionLoc = 0, ltPosLoc = 0;
    glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
	GLWrapper::errorCheck();
	const glm::mat4 *view = Singleton::getInstance()->getViewMat();
	proj = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
	modelLoc = glGetUniformLocation(m_program, "model");
	viewLoc = glGetUniformLocation(m_program, "view");
	projectionLoc = glGetUniformLocation(m_program, "projection");
	GLWrapper::errorCheck();
    glm::vec3 lightPos = glm::vec3(1.2,1.0,2.0);
    
    GLWrapper::errorCheck();
    GLint normLoc = glGetAttribLocation(m_program, "aNormal");
    glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(normLoc);

    GLint texLoc = glGetAttribLocation(m_program, "aTex");
    glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(texLoc);
    
	
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(*view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &proj[0][0]);

    GLWrapper::errorCheck();

    GLint offsetLoc = 0;
    glm::vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    std::string indexStr;
    std::stringstream ss;
    for (int y = -5; y < 5; y++)
    {
        for (int x = -5; x < 5; x++)
        {
            translations[index].x = x + offset;
            translations[index].y = y + offset;
			ss.clear();
            ss << index;
            ss >> indexStr;
            offsetLoc = glGetUniformLocation(m_program,("offsets[" + indexStr + "]").c_str());
            glUniform2fv(offsetLoc,1,glm::value_ptr(translations[index]));
            index++;
        }
    }
    GLWrapper::errorCheck();


    glGenTextures(1,&m_diffuseMap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_diffuseMap);
	GLWrapper::errorCheck();
	std::string path = CommonFunc::getResourceDirectory();
    int width, height, channels;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load((path + "/resources/images/container2.png").c_str(),&width,&height,&channels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,width,height,0, GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        //TODO: exception or debug info here
    }
	GLuint samplerPos = glGetUniformLocation(m_program, "material.diffuse");
	glUniform1i(samplerPos, 0);
    stbi_image_free(data);

    glGenTextures(1,&m_specularMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_specularMap);
    data = stbi_load((path + "/resources/images/container2_specular.png").c_str(),&width,&height,&channels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,width,height,0, GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        //TODO: exception or debug info here
    }
	samplerPos = glGetUniformLocation(m_program, "material.specular");
	glUniform1i(samplerPos, 1);
    stbi_image_free(data);

	GLWrapper::errorCheck();

	glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    glBindVertexArray(0);


    glGenVertexArrays(1,&m_shadowVAO);
    glBindVertexArray(m_shadowVAO);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    posLoc = glGetAttribLocation(Singleton::getInstance()->getShadowProgram(),"position");
    glVertexAttribPointer(posLoc,3,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(posLoc);
	GLWrapper::errorCheck();
}

void LightCube::render()
{
    glViewport(0, 0, 1280, 720);

    if(Singleton::getInstance()->getRenderTarget() == SHADOW)
    {
		GLWrapper::errorCheck();
        GLuint shadowPgm = Singleton::getInstance()->getShadowProgram();
        glUseProgram(shadowPgm);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glBindVertexArray(m_shadowVAO);
        glm::mat4 model = glm::mat4(1.0f);
        GLint modelLoc = glGetUniformLocation(shadowPgm, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
    }
    else
    {
        glUseProgram(m_program);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glBindVertexArray(m_vao);
        //GLint viewLoc = glGetUniformLocation(m_program, "viewMat");
        //const glm::mat4* view = Singleton::getInstance()->getViewMat();
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(*view));
        GLWrapper::errorCheck();
        GLint viewPosLoc = glGetUniformLocation(m_program,"viewPos");
        glUniform3fv(viewPosLoc,1,glm::value_ptr(Singleton::getInstance()->getCameraPosition()));

        GLint lightPosLoc = glGetUniformLocation(m_program,"light.lightVec");
        glUniform3fv(lightPosLoc,1,glm::value_ptr(Singleton::getInstance()->getCameraPosition()));

        //glm::mat4 proj = glm::mat4(1.0f);
        //proj = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
        //GLint projectionLoc = glGetUniformLocation(m_program, "projectionMat");
        //glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &proj[0][0]);
        
        GLWrapper::errorCheck();
        float shininess = 64;
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

        GLint constantLoc = glGetUniformLocation(m_program, "light.constant");
        glUniform1f(constantLoc, 1.0);
        GLint linearLoc = glGetUniformLocation(m_program, "light.linear");
        glUniform1f(linearLoc, 0.09);
        GLint quadraticLoc = glGetUniformLocation(m_program, "light.quadratic");
        glUniform1f(quadraticLoc, 0.032);

        GLint spotlightPosLoc = glGetUniformLocation(m_program, "light.position");
        glUniform3fv(spotlightPosLoc, 1,glm::value_ptr(Singleton::getInstance()->getLightPos()));
        GLint spotlightDirLoc = glGetUniformLocation(m_program, "light.direction");
        glUniform3fv(spotlightDirLoc, 1, glm::value_ptr(Singleton::getInstance()->getCameraFront()));
        GLint cutoffLoc = glGetUniformLocation(m_program, "light.cutoff");
        glUniform1f(cutoffLoc, glm::cos(glm::radians(12.5f)));
        GLint outerCutoffLoc = glGetUniformLocation(m_program, "light.outerCutoff");
        glUniform1f(outerCutoffLoc, glm::cos(glm::radians(17.5f)));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,m_specularMap);

        /*for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            GLint modelLoc = glGetUniformLocation(m_program, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/
        glDrawArrays(GL_TRIANGLES,0,36);
        //glDrawArraysInstanced(GL_TRIANGLES,0,36,100);
        
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glUseProgram(0);
    }
	
	
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

