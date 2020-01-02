#pragma once
#include "glm/glm.hpp"
#include "glad/glad.h"

class GLWrapper;

static const int SCR_WIDTH = 1280;
static const int SCR_HEIGHT = 720;

enum RENDER_TARGET
{
    SCENE,
    SHADOW
};

class Singleton
{
public:
    Singleton();
    ~Singleton();

    static Singleton* getInstance()
	{
		if (!s_instance)
			s_instance = new Singleton;
		return s_instance;
	};

    void setGLWrapper(GLWrapper* wrapper);

    void setViewMat(const glm::mat4 & mat);

	const glm::mat4* getViewMat();

    void setFOV(float fov);

    float getFOV();

    void setCameraPosition(const glm::vec3& camPos);

    glm::vec3 getCameraPosition();

    void setCameraFront(const glm::vec3& camFront);

    glm::vec3 getCameraFront();

    void setCameraUp(const glm::vec3& camUp);

    glm::vec3 getCameraUp();

    glm::vec3 getLightPos();

    glm::vec3 getParalellLightPos();

    void setLightPos(const glm::vec3& lightPos);

    glm::vec3 getLightDir();

    void setLightDir(const glm::vec3& lightDir);

    void setSkyboxTexture(GLuint texture);

    GLuint getSkyboxTexture();

    GLWrapper* getGLWrapper();

    //TODO: compatible with container later
    void setUboBlockId(GLuint id);

    GLuint getUboBlockId();

    void setShadowProgram(GLuint pgm);

    GLuint getShadowProgram();

    void setRenderTarget(RENDER_TARGET target);

    RENDER_TARGET getRenderTarget();

    void setTexVBO(GLuint texvbo);

    GLuint getTexVBO();

    void setTextureProgram(GLuint texpgm);

    GLuint getTextureProgram();

    void setDepthTexture(GLuint depTex);

    GLuint getDepthTexture();

    void setGBuffer(GLuint bufid);

    GLuint getGBuffer();

    void setGBufferPosTexture(GLuint gpos);

    GLuint getGBufferPosTexture();

    void setGBufferNormTexture(GLuint gnorm);

    GLuint getGBufferNormTexture();

    void setGBufferAlbedoSpec(GLuint aspec);

    GLuint getGBufferAlbedoSpec();

    void setDepthRBO(GLuint depthrbo);

    GLuint getDepthRBO();
private:

private:
    static Singleton*   s_instance;
    GLWrapper*          m_glWrapper;
	glm::mat4           m_viewMat;
    float               m_fov;
    glm::vec3           m_camPosition;
    glm::vec3           m_targetPos;
    glm::vec3           m_camDirection;
    glm::vec3           m_camRight;
    glm::vec3           m_camUp;
    glm::vec3           m_camFront;
    glm::vec3           m_lightPos;
    glm::vec3           m_lightDir;
    glm::vec3           m_paralellLightPos;
    GLuint              m_skyboxTexture;
    GLuint              m_uboId;
    GLuint              m_shadowProgram;
    RENDER_TARGET       m_renderTarget;
    GLuint              m_texVBO;
    GLuint              m_texturePgm;
    GLuint              m_depthTexture;
    GLuint              m_gBuffer;
    GLuint              m_gPosTex;
    GLuint              m_gNormTex;
    GLuint              m_gAlbedoSpec;
    GLuint              m_depthRBO;
};