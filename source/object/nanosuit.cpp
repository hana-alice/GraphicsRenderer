#include "Nanosuit.h"
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
#include <random>
#include "plane.h"

#include "stbimg/stb_image.h"
#include "Cube.h"
#include "thirdparty/model.h"
#pragma region vertices

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

char geoShader[sizeof(Shader)];
char lightShader[sizeof(Shader)];
char ssaoShader[sizeof(Shader)];
char ssaoBlur[sizeof(Shader)];
char model[sizeof(Model)];

std::vector<glm::vec3> lightPositions;
std::vector<glm::vec3> lightColors;
std::vector<glm::vec3> objectPositions;
std::vector<glm::vec3> ssaoKernel;
std::vector<glm::vec3> ssaoNoise;

GLfloat quadVerts[] =
{
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

#pragma endregion

GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
    if (quadVAO == 0)
    {
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

GLuint cubeVAO = 0;
GLuint cubeVBO = 0;
void RenderCube()
{
    // Initialize (if necessary)
    if (cubeVAO == 0)
    {
        GLfloat vertices[] = {
            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
            // Left face
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            // Right face
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
            // Bottom face
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            // Top face
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // Fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // Render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

GLfloat lerp(GLfloat min, GLfloat max, GLfloat f)
{
    return min + f * (max - min);
}

Nanosuit::Nanosuit():m_cube(nullptr),m_plane(nullptr)
{
}

Nanosuit::~Nanosuit()
{
}

void Nanosuit::init()
{
    std::string srcPath = CommonFunc::getResourceDirectory();
    std::ifstream vsSource,fsSource;

    Shader* geoPassShader = new(geoShader) Shader((srcPath + "/resources/shader/g_buffer.vs").c_str(),(srcPath + "/resources/shader/g_buffer.fs").c_str());
    Shader* lightPassShader = new(lightShader) Shader((srcPath + "/resources/shader/ds.vs").c_str(),(srcPath + "/resources/shader/ds.fs").c_str());
    Shader* ssao = new(ssaoShader) Shader((srcPath + "/resources/shader/ssao.vs").c_str(),(srcPath + "/resources/shader/ssao.fs").c_str());
    Shader* blur = new(ssaoBlur) Shader((srcPath + "/resources/shader/ssao.vs").c_str(),(srcPath + "/resources/shader/ssaoblur.fs").c_str());

    blur->use();
    glUniform1i(glGetUniformLocation(ssao->ID, "ssao"), 0);

    geoPassShader->use();
    unsigned int uniformBlockIndex = glGetUniformBlockIndex(geoPassShader->ID, "Matrices");
    glUniformBlockBinding(geoPassShader->ID, uniformBlockIndex, Singleton::getInstance()->getUboBlockId());
    GLWrapper::errorCheck();

    ssao->use();
    glUniform1i(glGetUniformLocation(ssao->ID, "gPositionDepth"), 0);
    glUniform1i(glGetUniformLocation(ssao->ID, "gNormal"), 1);
    glUniform1i(glGetUniformLocation(ssao->ID, "texNoise"), 2);

    lightPassShader->use();
    glUniform1i(glGetUniformLocation(lightPassShader->ID, "gPosition"), 0);
    glUniform1i(glGetUniformLocation(lightPassShader->ID, "gNormal"), 1);
    glUniform1i(glGetUniformLocation(lightPassShader->ID, "gAlbedoSpec"), 2);

    Model* cyborg = new(model) Model(srcPath + "/resources/model/nanosuit/nanosuit.obj");
    
    objectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
    objectPositions.push_back(glm::vec3(0.0, -3.0, -3.0));
    objectPositions.push_back(glm::vec3(3.0, -3.0, -3.0));
    objectPositions.push_back(glm::vec3(-3.0, -3.0, 0.0));
    objectPositions.push_back(glm::vec3(0.0, -3.0, 0.0));
    objectPositions.push_back(glm::vec3(3.0, -3.0, 0.0));
    objectPositions.push_back(glm::vec3(-3.0, -3.0, 3.0));
    objectPositions.push_back(glm::vec3(0.0, -3.0, 3.0));
    objectPositions.push_back(glm::vec3(3.0, -3.0, 3.0));
    
    
    srand(13);
    for (size_t i = 0; i < 32; i++)
    {
        GLfloat xpos = ((rand()%100)/100.0) * 6.0 - 3.0;
        GLfloat ypos = ((rand()%100)/100.0) * 6.0 - 3.0;
        GLfloat zpos = ((rand()%100)/100.0) * 6.0 - 3.0;
        lightPositions.push_back(glm::vec3(xpos,ypos,zpos));

        GLfloat r = ((rand()%100)/200.0) + 0.5f;
        GLfloat g = ((rand()%100)/200.0) + 0.5f;
        GLfloat b = ((rand()%100)/200.0) + 0.5f;
        lightColors.push_back(glm::vec3(r,g,b));
    }

    glGenVertexArrays(1, &m_shadingVAO);
    glBindVertexArray(m_shadingVAO);
    glUseProgram(lightPassShader->ID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferPosTexture());
    GLWrapper::errorCheck();
    GLint samplerPos = glGetUniformLocation(lightPassShader->ID, "gPosition");
    glUniform1i(samplerPos,0);
    GLWrapper::errorCheck();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferNormTexture());
    samplerPos = glGetUniformLocation(lightPassShader->ID, "gNormal");
    glUniform1i(samplerPos,1);
    GLWrapper::errorCheck();

    glActiveTexture(GL_TEXTURE2);
    GLWrapper::errorCheck();
    glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferAlbedoSpec());
    GLWrapper::errorCheck();
    samplerPos = glGetUniformLocation(lightPassShader->ID, "gAlbedoSpec");
    glUniform1i(samplerPos,2);

    glActiveTexture(GL_TEXTURE3);
    GLWrapper::errorCheck();
    glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferAlbedoSpec());
    GLWrapper::errorCheck();
    samplerPos = glGetUniformLocation(lightPassShader->ID, "ssao");
    glUniform1i(samplerPos, 3);

	GLWrapper::errorCheck();
    
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

    if (!m_plane)
    {
        m_plane = new Plane;
    }
    m_plane->init();
    
    //kernel
    std::uniform_real_distribution<GLfloat> randomFloats(0.0,1.0);
    std::default_random_engine generator;
    for (size_t i = 0; i < 64; i++)
    {
        glm::vec3 sample(
            randomFloats(generator) * 2.0f - 1.0f,  //x
            randomFloats(generator) * 2.0f - 1.0f,  //y
            randomFloats(generator));               //z
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        GLfloat scale = static_cast<GLfloat>(i)/64.0;
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }
    for (size_t i = 0; i < 16; i++)
    {
        glm::vec3 noise(
            randomFloats(generator) * 2.0f - 1.0f,
            randomFloats(generator) * 2.0f - 1.0f,
            0.0
        );
        ssaoNoise.push_back(noise);
    }
    glGenTextures(1, &m_noiseTexture);
    glBindTexture(GL_TEXTURE_2D, m_noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenFramebuffers(1, &m_ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);
    glGenTextures(1, &m_ssaoColorTex);
    glBindTexture(GL_TEXTURE_2D, m_ssaoColorTex); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoColorTex, 0);
    
    glGenFramebuffers(1, &m_blurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO);
    glGenTextures(1, &m_blurTex);
    glBindTexture(GL_TEXTURE_2D, m_blurTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_blurTex, 0);

    GLWrapper::errorCheck();
}

void Nanosuit::render()
{
    glViewport(0, 0, 1280, 720);


    GLWrapper::errorCheck();
    glBindFramebuffer(GL_FRAMEBUFFER,Singleton::getInstance()->getGBuffer());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    Model* cyborg = reinterpret_cast<Model*>(model);
            glm::mat4 model;
            Shader* shaderGeometryPass = reinterpret_cast<Shader*>(geoShader);
            shaderGeometryPass->use();
            
                model = glm::mat4();
                model = glm::rotate(model,-90.0f * glm::pi<float>()/180.0f,glm::vec3(1.0f,0.0f,0.0f));
                model = glm::scale(model, glm::vec3(0.25f));
                glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));
                cyborg->Draw(*shaderGeometryPass);

                model = glm::mat4();
                model = glm::translate(model, glm::vec3(0.0, -1.0f, 0.0f));
                model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
                glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));
                RenderCube();
            
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    GLWrapper::errorCheck();

    //ssao
    glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    Shader* shaderSSAO = reinterpret_cast<Shader*>(ssaoShader);
    shaderSSAO->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferPosTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferNormTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_noiseTexture);
    GLWrapper::errorCheck();
    for (GLuint i = 0; i < 64; ++i)
    {
        GLint pos = glGetUniformLocation(shaderSSAO->ID, ("samples[" + std::to_string(i) + "]").c_str());
        glUniform3fv(pos, 1, &ssaoKernel[i][0]);
        GLWrapper::errorCheck();
    }
    GLWrapper::errorCheck();
    glm::mat4 proj = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderSSAO->ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
    GLWrapper::errorCheck();
    RenderQuad();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLWrapper::errorCheck();

    //blur
    glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    Shader* blur = reinterpret_cast<Shader*>(ssaoBlur);
    blur->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ssaoColorTex);
    RenderQuad();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //shading
    glBindVertexArray(m_shadingVAO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Shader* lightPassShader = reinterpret_cast<Shader*>(lightShader);
    lightPassShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferPosTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferNormTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, Singleton::getInstance()->getGBufferAlbedoSpec());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_blurTex);

    for (size_t i = 0; i < lightPositions.size(); i++)
    {
        std::string str0 = "lights[" + std::to_string(i) + "].Position";
        std::string str1 = "lights[" + std::to_string(i) + "].Color";
        std::string str2 = "lights[" + std::to_string(i) + "].constant";
        std::string str3 = "lights[" + std::to_string(i) + "].linear";
        std::string str4 = "lights[" + std::to_string(i) + "].quadratic";
        GLint pp0 = glGetUniformLocation(lightPassShader->ID, str0.c_str());
        GLint pp1 = glGetUniformLocation(lightPassShader->ID, str1.c_str());
        GLint pp2 = glGetUniformLocation(lightPassShader->ID, str2.c_str());
        GLint pp3 = glGetUniformLocation(lightPassShader->ID, str3.c_str());
        GLint pp4 = glGetUniformLocation(lightPassShader->ID, str4.c_str());
        glm::vec4 lightpos = glm::vec4(lightPositions[i].x, lightPositions[i].y, lightPositions[i].z, 1.0);
        glUniform3fv(glGetUniformLocation(lightPassShader->ID,("lights[" + std::to_string(i) + "].Position").c_str()),1, glm::value_ptr(glm::vec3((*Singleton::getInstance()->getViewMat()) * lightpos)) );
        glUniform3fv(glGetUniformLocation(lightPassShader->ID,("lights[" + std::to_string(i) + "].Color").c_str()),1, &lightColors[i][0]);

        const GLfloat constant = 1.0;
        const GLfloat linear = 0.7;
        const GLfloat quadratic = 1.8;
        //glUniform1f(glGetUniformLocation(lightPassShader->ID,("lights[" + std::to_string(i) + "].constant").c_str()), constant);
        glUniform1f(glGetUniformLocation(lightPassShader->ID,("lights[" + std::to_string(i) + "].linear").c_str()), linear);
        glUniform1f(glGetUniformLocation(lightPassShader->ID,("lights[" + std::to_string(i) + "].quadratic").c_str()), quadratic);
    }
    glm::vec3 campos = Singleton::getInstance()->getCameraPosition();
    glUniform3fv(glGetUniformLocation(lightPassShader->ID,"viewPos"),1,glm::value_ptr(Singleton::getInstance()->getCameraPosition()));
    GLWrapper::errorCheck();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    
#if 0
    glBindFramebuffer(GL_READ_FRAMEBUFFER, Singleton::getInstance()->getGBuffer());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0,0,SCR_WIDTH,SCR_HEIGHT,0,0,SCR_WIDTH,SCR_HEIGHT,GL_DEPTH_BUFFER_BIT,GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    m_plane->render();
    //for (size_t i = 0; i < lightPositions.size(); i++)
    //{
    //    m_cube->setModelMatrix(glm::translate(glm::mat4(1.0), lightPositions[i]));
    //    m_cube->setColor(glm::vec4(lightColors[i], 1.0));
    //    m_cube->render();
    //}
#endif	
}

void Nanosuit::destroy()
{
    //TODO later
}

