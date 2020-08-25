#include "PBRDemo.h"
#include "Singleton.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLWrapper.h"
#include <fstream>
#include <string>
#include "CommonFunc.h"
#include <vector>
#include "util.h"
#include "glfw/glfw3.h"
#include "stbimg/stb_image.h"
#include <iostream>

glm::vec3 lightPositions[] = {
    glm::vec3(-10.0f,  10.0f, 10.0f),
    glm::vec3( 10.0f,  10.0f, 10.0f),
    glm::vec3(-10.0f, -10.0f, 10.0f),
    glm::vec3( 10.0f, -10.0f, 10.0f),
};
glm::vec3 lightColors[] = {
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f),
    glm::vec3(300.0f, 300.0f, 300.0f)
};

void renderCube();
void renderQuad();

PBRDemo::PBRDemo() : m_indexCount(0)
{

}

PBRDemo::~PBRDemo()
{
    glDetachShader(m_program, m_vertexShader);
	glDetachShader(m_program, m_fragmentShader);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteProgram(m_program);
}

void PBRDemo::init()
{
	prepareCube();
    prepareVerts();
}

void PBRDemo::prepareVerts()
{
    std::string srcPath = CommonFunc::getResourceDirectory();
    std::ifstream vsSource,fsSource;

    vsSource.open(srcPath + "/resources/shader/pbr.vs");
    fsSource.open(srcPath + "/resources/shader/pbr.fs");
    
	std::string bufStr;
	std::string vs, fs, gs;
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

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    const uint32_t X_SEG = 64;
    const uint32_t Y_SEG = 64;
    for (size_t y = 0; y <= Y_SEG; ++y)
    {
        for (size_t x = 0; x <= X_SEG; ++x)
        {
            float xseg = (float)x / (float)X_SEG;
            float yseg = (float)y / (float)Y_SEG;
            float xPos = std::cos(xseg * 2.0f * PI) * std::sin(yseg * PI);
            float yPos = std::cos(yseg * PI);
            float zPos = std::sin(xseg * 2.0f * PI) * std::sin(yseg * PI);

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uvs.push_back(glm::vec2(xseg, yseg));
            normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    bool oddRow = false;
    for (size_t y = 0; y < Y_SEG; ++y)
    {
        if (!oddRow)
        {
            for (size_t x = 0; x <= X_SEG; ++x)
            {
                indices.push_back(y       * (X_SEG + 1) + x);
                indices.push_back((y + 1) * (Y_SEG + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEG; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEG + 1) + x);
                indices.push_back(y       * (Y_SEG + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    m_indexCount = indices.size();
    std::vector<float> data;
    data.reserve(positions.size() + uvs.size() + normals.size());
	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (uvs.size() > 0)
		{
			data.push_back(uvs[i].x);
			data.push_back(uvs[i].y);
		}
		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

	GLWrapper::errorCheck();
	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glUseProgram(m_program);
	GLint posLoc = 0, normLoc = 0, uvLoc = 0, modelLoc = 0, viewLoc = 0, projectionLoc = 0;
	posLoc = glGetAttribLocation(m_program, "aPos");
    normLoc = glGetAttribLocation(m_program, "aNormal");
    //uvLoc = glGetAttribLocation(m_program, "aTexCoords");

	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(uvLoc);
	//glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(normLoc);
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5*sizeof(float)));
	GLWrapper::errorCheck();
	glm::mat4 model = glm::mat4(1.0f);
	//view = Singleton::getInstance()->getViewMat();
	//proj = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
	modelLoc = glGetUniformLocation(m_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    unsigned int uniformBlockIndex = glGetUniformBlockIndex(m_program,"Matrices");
    glUniformBlockBinding(m_program,uniformBlockIndex,Singleton::getInstance()->getUboBlockId());
	GLWrapper::errorCheck();
    GLuint smpLoc = glGetUniformLocation(m_program, "irradianceMap");
    glUniform1i(smpLoc, 0);
    GLuint prefLoc = glGetUniformLocation(m_program, "prefilterMap");
    glUniform1i(prefLoc, 1);
    GLuint brdfLoc = glGetUniformLocation(m_program, "brdfLUT");
    glUniform1i(brdfLoc, 2);
	GLWrapper::errorCheck();
    GLuint aoLoc = glGetUniformLocation(m_program, "ao");
    glUniform1f(aoLoc, 1.0f);
	GLWrapper::errorCheck();
    GLuint albedoLoc = glGetUniformLocation(m_program, "albedo");
    glUniform3f(albedoLoc, 0.5f, 0.0f, 0.0f);
	GLWrapper::errorCheck();
	glUseProgram(0);
	glBindVertexArray(0);

	GLWrapper::errorCheck();
}

void PBRDemo::render()
{    
    glViewport(0, 0, 1280, 720);
	glUseProgram(m_program);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(m_vao);
    GLuint camLoc = glGetUniformLocation(m_program, "camPos");
    glUniform3fv(camLoc, 1, glm::value_ptr(Singleton::getInstance()->getCameraPosition()));
	GLWrapper::errorCheck();
    int rows = 7;
	int columns = 7;
    float space = 2.5;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_brdfLUTTexture);

    for (size_t row = 0; row < rows; row++)
    {
        GLuint metalLoc = glGetUniformLocation(m_program, "metallic");
		float metallic = (float)row / (float)rows;
        glUniform1f(metalLoc, (float)row / (float)rows);
		GLWrapper::errorCheck();
        for (size_t column = 0; column < columns; column++)
        {
            GLuint roughLoc = glGetUniformLocation(m_program, "roughness");
            float roughness = glm::clamp((float)column / (float)columns, 0.05f, 1.0f);
            glUniform1f(roughLoc, roughness);
			GLWrapper::errorCheck();
            glm::mat4 model = glm::mat4(1.0f);
            GLuint modelLoc = glGetUniformLocation(m_program, "model");
            model = glm::translate(model, glm::vec3(
                (column - (columns / 2.0f)) * space,
                (row - (rows / 2.0f)) * space,
                0.0f
            ));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			GLWrapper::errorCheck();
			glDrawElements(GL_TRIANGLE_STRIP, m_indexCount, GL_UNSIGNED_INT, 0);
			GLWrapper::errorCheck();
            
        }
		
    }
	GLWrapper::errorCheck();
    for (size_t i = 0; i < sizeof(lightPositions)/sizeof(lightPositions[0]); i++)
    {
        glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        newPos = lightPositions[i];
        GLint lightLoc = glGetUniformLocation(m_program, ("lightPositions[" + std::to_string(i) + "]").c_str());
        glUniform3fv(lightLoc, 1, glm::value_ptr(newPos));
        GLint colorLoc = glGetUniformLocation(m_program, ("lightColors[" + std::to_string(i) + "]").c_str());
        glUniform3fv(colorLoc, 1, glm::value_ptr(lightColors[i]));
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, newPos);
        model = glm::scale(model, glm::vec3(0.5f));
        GLuint modelLoc = glGetUniformLocation(m_program, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLE_STRIP, m_indexCount, GL_UNSIGNED_INT, 0);
    }
	glBindVertexArray(0);
    GLWrapper::errorCheck();

    glUseProgram(m_skyboxPgm);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubeTex);
    renderCube(); 
	GLWrapper::errorCheck();
	glDisable(GL_DEPTH_TEST);
	glUseProgram(0);

    /* static int count = 0;
    static GLuint pgm = 0;
    if (count < 1)
    {
        std::string srcPath = CommonFunc::getResourceDirectory();
        pgm   = loadShader((srcPath + "/resources/shader/texturev2.vs").c_str(), (srcPath + "/resources/shader/texturev2.fs").c_str());
        glUseProgram(pgm);
        GLint smp = glGetUniformLocation(pgm, "texCanvas");
        glUniform1i(smp, 0);
		GLWrapper::errorCheck();
        count++;
    }
    glUseProgram(pgm);
    glActiveTexture(GL_TEXTURE0);
	GLWrapper::errorCheck();
    glBindTexture(GL_TEXTURE_2D, m_brdfLUTTexture);
	GLWrapper::errorCheck();
    renderQuad();
    GLWrapper::errorCheck();
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);  */
    //glUseProgram(m_brdfPgm);
    //renderQuad();

}

GLuint PBRDemo::loadShader(const char* vsPath, const char* fsPath)
{
    std::string srcPath = CommonFunc::getResourceDirectory();
    std::ifstream vsSource,fsSource;

    vsSource.open(vsPath);
    fsSource.open(fsPath);
    
	std::string bufStr;
	std::string vs, fs, gs;
	while (getline(vsSource, bufStr))
	{
		vs += (bufStr + '\n');
	}
	while (getline(fsSource, bufStr))
	{
		fs += (bufStr + '\n');
	}
    vsSource.close();
    fsSource.close();

    GLuint vertexShader, fragmentShader;

    m_glWrapper = Singleton::getInstance()->getGLWrapper();
    vertexShader = m_glWrapper->createShader(GL_VERTEX_SHADER,vs.c_str());
    fragmentShader = m_glWrapper->createShader(GL_FRAGMENT_SHADER,fs.c_str());
    
    GLuint pgm = m_glWrapper->createProgram(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

    return pgm;
}

void PBRDemo::prepareCube()
{
    std::string srcPath = CommonFunc::getResourceDirectory();
    std::ifstream vsSource,fsSource;

    m_cubePgm   = loadShader((srcPath + "/resources/shader/cubemap.vs").c_str(), (srcPath + "/resources/shader/cubemap.fs").c_str());
    m_skyboxPgm = loadShader((srcPath + "/resources/shader/pbrskybox.vs").c_str(), (srcPath + "/resources/shader/pbrskybox.fs").c_str());
    m_brdfPgm   = loadShader((srcPath + "/resources/shader/brdf.vs").c_str(), (srcPath + "/resources/shader/brdf.fs").c_str());
    m_convolutionalPgm  = loadShader((srcPath + "/resources/shader/cubemap.vs").c_str(), (srcPath + "/resources/shader/convolutionalEnv.fs").c_str());
    m_prefilterPgm      = loadShader((srcPath + "/resources/shader/cubemap.vs").c_str(), (srcPath + "/resources/shader/prefilter.fs").c_str());

    glUseProgram(m_skyboxPgm);
    GLint envSampler = glGetUniformLocation(m_skyboxPgm, "environmentMap");
	GLWrapper::errorCheck();
    unsigned int uniformBlockIndex = glGetUniformBlockIndex(m_skyboxPgm,"Matrices");
    glUniformBlockBinding(m_skyboxPgm,uniformBlockIndex,Singleton::getInstance()->getUboBlockId());
	GLWrapper::errorCheck();
    glUniform1i(envSampler, 0);
    glUseProgram(0);
	GLWrapper::errorCheck();

    GLuint captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	GLWrapper::errorCheck();
    m_hdrTex = loadTexture((srcPath + "/resources/images/kloppenheim_02_2k.hdr").c_str(),true);
	GLWrapper::errorCheck();
    glGenTextures(1, &m_envCubeTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubeTex);
    for (size_t i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLWrapper::errorCheck();
    glm::mat4 captureProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
	GLWrapper::errorCheck();

    glUseProgram(m_cubePgm);
    GLint erSamplerLoc = glGetUniformLocation(m_cubePgm, "equirectangularMap");
    glUniform1i(erSamplerLoc, 0);
    GLint projLoc = glGetUniformLocation(m_cubePgm, "projectionMat");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(captureProj));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_hdrTex);
    glViewport(0, 0, 512, 512);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    for (size_t i = 0; i < 6; i++)
    {
        GLint viewLoc = glGetUniformLocation(m_cubePgm, "viewMat");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(captureViews[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_envCubeTex, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube();    
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubeTex);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	GLWrapper::errorCheck();

    glGenTextures(1, &m_irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMap);
    for (size_t i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
	GLWrapper::errorCheck();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
    glUseProgram(m_convolutionalPgm);
    GLint envSamplerLoc = glGetUniformLocation(m_convolutionalPgm, "envMap");
    glUniform1i(envSamplerLoc, 0);
    projLoc = glGetUniformLocation(m_convolutionalPgm, "projectionMat");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(captureProj));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubeTex);
    glViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (size_t i = 0; i < 6; i++)
    {
        GLint viewLoc = glGetUniformLocation(m_convolutionalPgm, "viewMat");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(captureViews[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube();    
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLWrapper::errorCheck();

    glGenTextures(1, &m_prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMap);
    for (size_t i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    GLWrapper::errorCheck();
    glUseProgram(m_prefilterPgm);
    GLint envCubeSmp = glGetUniformLocation(m_prefilterPgm, "envMap");
    glUniform1i(envCubeSmp, 0);
    projLoc = glGetUniformLocation(m_prefilterPgm, "projectionMat") ;
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(captureProj));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubeTex);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    uint8_t maxMipmapLevels = 5;
    GLWrapper::errorCheck();
    for (size_t mip = 0; mip < maxMipmapLevels; mip++)
    {
        uint32_t mipWidth   = 128 * pow(0.5, mip);
        uint32_t mipHeight  = 128 * pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = float(mip) / float(maxMipmapLevels - 1);
        GLint roughLoc = glGetUniformLocation(m_prefilterPgm, "roughness");
        glUniform1f(roughLoc, roughness);
        for (size_t i = 0; i < 6; i++)
        {
            GLint viewLoc = glGetUniformLocation(m_prefilterPgm, "viewMat");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(captureViews[i]));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_prefilterMap, mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderCube();
        }
    }
    GLWrapper::errorCheck();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenTextures(1, &m_brdfLUTTexture);
    glBindTexture(GL_TEXTURE_2D, m_brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brdfLUTTexture, 0);
    GLWrapper::errorCheck();
    glViewport(0, 0, 512, 512);
    glUseProgram(m_brdfPgm);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderQuad();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLWrapper::errorCheck();
}

unsigned int PBRDemo::loadTexture(char const * path, bool hdr)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
	void *data = nullptr;
	if (hdr)
	{
		data = stbi_loadf(path, &width, &height, &nrComponents, 0);
	}
	else
		data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        if(hdr)
        {
			GLWrapper::errorCheck();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
            GLWrapper::errorCheck();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			GLWrapper::errorCheck();
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void PBRDemo::destroy()
{
	delete this;
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void renderCube()
{
	static unsigned int cubeVAO = 0;
	static unsigned int cubeVBO = 0;
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
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
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}