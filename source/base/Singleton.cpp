#include "Singleton.h"

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