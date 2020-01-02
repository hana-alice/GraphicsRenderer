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
#include <vector>

#include "stbimg/stb_image.h"
#include "Cube.h"
#pragma region vertices
static float vertices[] = {
        // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
    };

static glm::vec3 cubePositions[] = {
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

std::vector<glm::vec3> lightPositions;
std::vector<glm::vec3> lightColors;

GLfloat quadVerts[] =
{
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

#pragma endregion

LightCube::LightCube():m_cube(nullptr)
{
}

LightCube::~LightCube()
{
}

void LightCube::init()
{
    std::string srcPath = CommonFunc::getResourceDirectory();
    std::ifstream vsSource,fsSource;
    
    vsSource.open(srcPath + "/resources/shader/gbuffer.vs");
    fsSource.open(srcPath + "/resources/shader/gbuffer.fs");
    
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
    
    m_gbufProgram = glCreateProgram();
    glAttachShader(m_gbufProgram, m_vertexShader);
    glAttachShader(m_gbufProgram, m_fragmentShader);
    glBindAttribLocation(m_gbufProgram, 0, "aPos");
    glBindAttribLocation(m_gbufProgram, 1, "aNorm");
    glBindAttribLocation(m_gbufProgram, 2, "aTex");
    GLWrapper::errorCheck();
    glLinkProgram(m_gbufProgram);
    GLWrapper::errorCheck();
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
    GLWrapper::errorCheck();
    vsSource.close();
    fsSource.close();
    vs.clear();
    fs.clear();
    bufStr.clear();

    vsSource.open(srcPath + "/resources/shader/ds.vs");
    fsSource.open(srcPath + "/resources/shader/ds.fs");

    while (getline(vsSource, bufStr))
    {
        vs += (bufStr + '\n');
    }
    while (getline(fsSource, bufStr))
    {
        fs += (bufStr + '\n');
    }
    m_vertexShader = m_glWrapper->createShader(GL_VERTEX_SHADER, vs.c_str());
    m_fragmentShader = m_glWrapper->createShader(GL_FRAGMENT_SHADER, fs.c_str());
    m_shadingProgram = m_glWrapper->createProgram(m_vertexShader, m_fragmentShader);

    glGenVertexArrays(1,&m_gBufVAO);
    glBindVertexArray(m_gBufVAO);
    glGenBuffers(1,&m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    unsigned int uniformBlockIndex = glGetUniformBlockIndex(m_gbufProgram,"Matrices");
    glUniformBlockBinding(m_gbufProgram,uniformBlockIndex,Singleton::getInstance()->getUboBlockId());
    GLWrapper::errorCheck();
    GLint posLoc = glGetAttribLocation(m_gbufProgram,"aPos");
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(0);

    GLint normLoc = glGetAttribLocation(m_gbufProgram, "aNorm");
    glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(normLoc);

    GLint texLoc = glGetAttribLocation(m_gbufProgram, "aTex");
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);
    GLWrapper::errorCheck();

    glUseProgram(m_gbufProgram);
    GLint modelLoc = 0, viewLoc = 0, projectionLoc = 0, ltPosLoc = 0;
    glm::mat4 model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(m_gbufProgram, "modelMat");
    GLWrapper::errorCheck();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

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
	GLuint samplerPos = glGetUniformLocation(m_gbufProgram, "texture_diffuse");
    GLWrapper::errorCheck();
	glUniform1i(samplerPos, 0);
    stbi_image_free(data);
    GLWrapper::errorCheck();
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
	samplerPos = glGetUniformLocation(m_gbufProgram, "texture_specular");
	glUniform1i(samplerPos, 1);
    stbi_image_free(data);
    GLWrapper::errorCheck();

    GLint offsetLoc = 0;
    glm::vec3 translations[1000];
    int index = 0;
    float offset = 0.6f;
    std::string indexStr;
    std::stringstream ss;
	for (int z = -5; z < 5; z++)
	{
		for (int y = -5; y < 5; y++)
		{
			for (int x = -5; x < 5; x++)
			{
				translations[index].x = x + offset;
				translations[index].y = y + offset;
				translations[index].z = z + offset;
				ss.clear();
				ss << index;
				ss >> indexStr;
				offsetLoc = glGetUniformLocation(m_gbufProgram, ("offsets[" + indexStr + "]").c_str());
				glUniform3fv(offsetLoc, 1, glm::value_ptr(translations[index]));
				index++;
			}
		}
	}
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);



    glGenVertexArrays(1, &m_shadingVAO);
    glBindVertexArray(m_shadingVAO);
    glUseProgram(m_shadingProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferPosTexture());
    GLWrapper::errorCheck();
    samplerPos = glGetUniformLocation(m_shadingProgram, "gPosition");
    glUniform1i(samplerPos,0);
    GLWrapper::errorCheck();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferNormTexture());
    samplerPos = glGetUniformLocation(m_shadingProgram, "gNormal");
    glUniform1i(samplerPos,1);
    GLWrapper::errorCheck();

    glActiveTexture(GL_TEXTURE2);
    GLWrapper::errorCheck();
    glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferAlbedoSpec());
    GLWrapper::errorCheck();
    samplerPos = glGetUniformLocation(m_shadingProgram, "gAlbedoSpec");
    glUniform1i(samplerPos,2);

	GLWrapper::errorCheck();

	

    srand(13);
    for (size_t i = 0; i < 32; i++)
    {
        GLfloat xpos = ((rand()%100)/100.0) * 20.0 - 10.0;
        GLfloat ypos = ((rand()%100)/100.0) * 20.0 - 10.0;
        GLfloat zpos = ((rand()%100)/100.0) * 20.0 - 10.0;
        lightPositions.push_back(glm::vec3(xpos,ypos,zpos));

        GLfloat r = ((rand()%100)/200.0) + 0.5f;
        GLfloat g = ((rand()%100)/200.0) + 0.5f;
        GLfloat b = ((rand()%100)/200.0) + 0.5f;
        lightColors.push_back(glm::vec3(r,g,b));
    }
    glGenBuffers(1, &m_quadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), &quadVerts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
    glBindVertexArray(0);
    glUseProgram(0);
    
    if (!m_cube)
    {
        m_cube = new Cube;
    }
    m_cube->init();
    
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
		GLWrapper::errorCheck();
        glBindVertexArray(m_shadowVAO);
		GLWrapper::errorCheck();
        glm::mat4 model = glm::mat4(1.0f);
        GLint modelLoc = glGetUniformLocation(shadowPgm, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glUseProgram(0);
		GLWrapper::errorCheck();
    }
    else
    {
        GLWrapper::errorCheck();
        glBindFramebuffer(GL_FRAMEBUFFER,Singleton::getInstance()->getGBuffer());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(m_gbufProgram);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glBindVertexArray(m_gBufVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_specularMap);
        glDrawArraysInstanced(GL_TRIANGLES,0,36,1000);
        glBindVertexArray(0);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        GLWrapper::errorCheck();


        //shading
        glBindVertexArray(m_shadingVAO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(m_shadingProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferPosTexture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferNormTexture());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferAlbedoSpec());
        for (size_t i = 0; i < lightPositions.size(); i++)
        {
            std::string str0 = "lights[" + std::to_string(i) + "].Position";
            std::string str1 = "lights[" + std::to_string(i) + "].Color";
            std::string str2 = "lights[" + std::to_string(i) + "].constant";
            std::string str3 = "lights[" + std::to_string(i) + "].linear";
            std::string str4 = "lights[" + std::to_string(i) + "].quadratic";
            GLint pp0 = glGetUniformLocation(m_shadingProgram, str0.c_str());
            GLint pp1 = glGetUniformLocation(m_shadingProgram, str1.c_str());
            GLint pp2 = glGetUniformLocation(m_shadingProgram, str2.c_str());
            GLint pp3 = glGetUniformLocation(m_shadingProgram, str3.c_str());
            GLint pp4 = glGetUniformLocation(m_shadingProgram, str4.c_str());
            glUniform3fv(glGetUniformLocation(m_shadingProgram,("lights[" + std::to_string(i) + "].Position").c_str()),1, &lightPositions[i][0]);
            glUniform3fv(glGetUniformLocation(m_shadingProgram,("lights[" + std::to_string(i) + "].Color").c_str()),1, &lightColors[i][0]);

            const GLfloat constant = 1.0;
            const GLfloat linear = 0.7;
            const GLfloat quadratic = 1.8;
            //glUniform1f(glGetUniformLocation(m_shadingProgram,("lights[" + std::to_string(i) + "].constant").c_str()), constant);
            glUniform1f(glGetUniformLocation(m_shadingProgram,("lights[" + std::to_string(i) + "].linear").c_str()), linear);
            glUniform1f(glGetUniformLocation(m_shadingProgram,("lights[" + std::to_string(i) + "].quadratic").c_str()), quadratic);
        }
        glm::vec3 campos = Singleton::getInstance()->getCameraPosition();
        glUniform3fv(glGetUniformLocation(m_shadingProgram,"viewPos"),1,glm::value_ptr(Singleton::getInstance()->getCameraPosition()));
        GLWrapper::errorCheck();
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, Singleton::getInstance()->getGBuffer());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0,0,SCR_WIDTH,SCR_HEIGHT,0,0,SCR_WIDTH,SCR_HEIGHT,GL_DEPTH_BUFFER_BIT,GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    for (size_t i = 0; i < lightPositions.size(); i++)
    {
        m_cube->setModelMatrix(glm::translate(glm::mat4(1.0), lightPositions[i]));
        m_cube->setColor(glm::vec4(lightColors[i], 1.0));
        m_cube->render();

    }
    
	
}

void LightCube::destroy()
{
    glDetachShader(m_gbufProgram, m_vertexShader);
	glDetachShader(m_gbufProgram, m_fragmentShader);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteProgram(m_gbufProgram);
}

