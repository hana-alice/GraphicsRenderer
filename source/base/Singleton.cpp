#include "Singleton.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"

Singleton* Singleton::s_instance = NULL;
Singleton::Singleton()
:m_glWrapper(nullptr),
m_lightPos(1.0,1.0,1.0),
m_lightDir(-0.2f, -1.0f, -0.3f)
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

    m_paralellLightPos = glm::vec3(-2.0f, 4.0f, -1.0f);

	//m_camPosition = m_paralellLightPos;
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

glm::vec3 Singleton::getLightDir()
{
    return m_lightDir;
}

void Singleton::setLightDir(const glm::vec3& lightDir)
{
    m_lightDir = lightDir;
}

GLuint Singleton::getSkyboxTexture()
{
    return m_skyboxTexture;
}

void Singleton::setSkyboxTexture(GLuint tex)
{
    m_skyboxTexture = tex;
}

void Singleton::setUboBlockId(GLuint id)
{
    m_uboId = id;
}

GLuint Singleton::getUboBlockId()
{
    return m_uboId;
}

glm::vec3 Singleton::getParalellLightPos()
{
    return m_paralellLightPos;
}

GLuint Singleton::getShadowProgram()
{
    return m_shadowProgram;
}

void Singleton::setShadowProgram(GLuint pgm)
{
    m_shadowProgram = pgm;
}

void Singleton::setRenderTarget(RENDER_TARGET target)
{
    m_renderTarget = target;
}

RENDER_TARGET Singleton::getRenderTarget()
{
    return m_renderTarget;
}

void Singleton::setTexVBO(GLuint texVBO)
{
    m_texVBO = texVBO;
}

GLuint Singleton::getTexVBO()
{
    return m_texVBO;
}

void Singleton::setDepthTexture(GLuint tex)
{
    m_depthTexture = tex;
}

GLuint Singleton::getDepthTexture()
{
    return m_depthTexture;
}

void Singleton::setTextureProgram(GLuint pgm)
{
    m_texturePgm = pgm;
}

GLuint Singleton::getTextureProgram()
{
    return m_texturePgm;
}

void Singleton::setGBuffer(GLuint buf)
{
    m_gBuffer = buf;
}

GLuint Singleton::getGBuffer()
{
    return m_gBuffer;
}

void Singleton::setGBufferPosTexture(GLuint gpos)
{
    m_gPosTex = gpos;
}

GLuint Singleton::getGBufferPosTexture()
{
    return m_gPosTex;
}

void Singleton::setGBufferNormTexture(GLuint gnorm)
{
    m_gNormTex = gnorm;
}

GLuint Singleton::getGBufferNormTexture()
{
    return m_gNormTex;
}

void Singleton::setGBufferAlbedoSpec(GLuint aspec)
{
    m_gAlbedoSpec = aspec;
}

GLuint Singleton::getGBufferAlbedoSpec()
{
    return m_gAlbedoSpec;
}

void Singleton::setDepthRBO(GLuint depthrbo)
{
    m_depthRBO = depthrbo;
}

GLuint Singleton::getDepthRBO()
{
    return m_depthRBO;
}

void Singleton::setGbufferPosDepth(GLuint posDepth)
{
    m_gPosDepth = posDepth;
}

GLuint Singleton::getGBufferPosDepth()
{
    return m_gPosDepth;
}