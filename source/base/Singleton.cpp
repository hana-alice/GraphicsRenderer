#include "Singleton.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"

Singleton* Singleton::s_instance = NULL;
Singleton::Singleton()
:m_glWrapper(nullptr)
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

glm::mat4 Singleton::getViewMat()
{
    float radius = 10.0;
    float camZ = cos(glfwGetTime()) * radius;
    float camX = sin(glfwGetTime()) * radius;
    m_viewMat = glm::lookAt(glm::vec3(camX,0.0,camZ),m_targetPos,glm::vec3(0.0,1.0,0.0));
    return m_viewMat;
}

void Singleton::setFOV(float fov)
{
    m_fov = fov;
}

float Singleton::getFOV()
{
    return m_fov;
}