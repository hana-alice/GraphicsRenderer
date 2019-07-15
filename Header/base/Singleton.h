#pragma once
#include "glm/glm.hpp"

class GLWrapper;

static const int SCR_WIDTH = 1280;
static const int SCR_HEIGHT = 720;

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

    void setLightPos(const glm::vec3& lightPos);

    glm::vec3 getLightDir();

    void setLightDir(const glm::vec3& lightDir);

    void setSkyboxTexture(unsigned int texture);

    unsigned int getSkyboxTexture();

    GLWrapper* getGLWrapper();

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
    unsigned int        m_skyboxTexture;
};