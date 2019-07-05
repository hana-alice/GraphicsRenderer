#include "Singleton.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"

Singleton* Singleton::s_instance = NULL;
Singleton::Singleton()
:m_glWrapper(nullptr),
m_lightPos(1.0,1.0,1.0)
{
    m_viewMat = glm::mat4(1.0f);
    m_viewMat = glm::translate(m_viewMat, glm::vec3(0.0f, 0.0f, -3.0f));
	m_fov = 45.0f;
    m_camPosition = glm::vec3(0.0,3.0,3.0);
    m_targetPos = glm::vec3(0,0,0);
    m_camDirection = glm::normalize(m_camPosition - m_targetPos);

    glm::vec3 up = glm::vec3(0.0,1.0,0.0);
    m_camRight = glm::normalize(glm::cross(up,m_camDirection));
    m_camUp = glm::cross(glm::normalize(m_camDirection),m_camRight);

    m_camFront = glm::vec3(0.0,0.0,-1.0);
	m_viewMat = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}
Singleton::~Singleton()
{

}

void Singleton::setGLWrapper(GLWrapper* wrapper)
{
    m_glWrapper = wrapper;
}

GLWrapper* Singleton::getGLWrapper()
{
    return m_glWrapper;
}

void Singleton::setViewMat(const glm::mat4 & mat)
{
    m_viewMat = mat;
}

const glm::mat4* Singleton::getViewMat()
{
    //to assure every time campos changes, direction towards front but not a solid point
    //"front" here means we eyes see towards inside the screen, that is minus Z.
    m_viewMat = glm::lookAt(m_camPosition,m_camPosition + m_camFront,glm::vec3(0.0,1.0,0.0));
    return &m_viewMat;
}

void Singleton::setFOV(float fov)
{
    m_fov = fov;
}

float Singleton::getFOV()
{
    return m_fov;
}

void Singleton::setCameraPosition(const glm::vec3& camPos)
{
    m_camPosition = camPos;
}

glm::vec3 Singleton::getCameraPosition()
{
    return m_camPosition;
}

void Singleton::setCameraFront(const glm::vec3& camFront)
{
    m_camFront = camFront;
}

glm::vec3 Singleton::getCameraFront()
{
    return m_camFront;
}

void Singleton::setCameraUp(const glm::vec3& camUp)
{
    m_camUp = camUp;
}

glm::vec3 Singleton::getCameraUp()
{
    return m_camUp;
}

glm::vec3 Singleton::getLightPos()
{
    return m_lightPos;
}

void Singleton::setLightPos(const glm::vec3& lightPos)
{
    m_lightPos = lightPos;
}