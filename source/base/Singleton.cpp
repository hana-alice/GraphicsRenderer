#include "Singleton.h"

Singleton* Singleton::s_instance = NULL;
Singleton::Singleton()
:m_glWrapper(nullptr)
{

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