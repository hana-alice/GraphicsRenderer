#include "GLWrapper.h"

GLWrapper::GLWrapper()
{

}

GLWrapper::~GLWrapper()
{

}

bool GLWrapper::registerInitFunc(InitFunc f)
{
    if(std::find(m_initFuncVec.begin(),m_initFuncVec.end(),f) != m_initFuncVec.end())
        m_initFuncVec.push_back(f);
    else
        return false;
    return true;
}

bool GLWrapper::registerRenderFunc(RenderFunc f)
{
    if(std::find(m_renderFuncVec.begin(),m_renderFuncVec.end(),f) != m_renderFuncVec.end())
        m_renderFuncVec.push_back(f);
    else
        return false;
    return true;
}

void GLWrapper::init()
{
    for(InitFunc f : m_initFuncVec)
        f();
}

void GLWrapper::render()
{
    for (RenderFunc f : m_renderFuncVec)
        f();
}
