#include "Singleton.h"
#include "glm/gtc/matrix_transform.hpp"

Singleton* Singleton::s_instance = NULL;
Singleton::Singleton()
:m_glWrapper(nullptr)
{
    m_viewMat = glm::mat4(1.0f);
    m_viewMat = glm::translate(m_viewMat, glm::vec3(0.0f, 0.0f, -3.0f));
	m_fov = 45.0f;
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