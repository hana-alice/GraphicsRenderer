#pragma once
#include "glad\glad.h"

class GLWrapper;
class Cube;
class LightCube
{
public:

    LightCube();
    ~LightCube();

    void init();

	void render();

	void destroy();
private:
    GLWrapper* m_glWrapper;
	GLuint m_vertexShader;
	GLuint m_fragmentShader;
	GLuint m_gbufProgram;
	GLuint m_shadingProgram;
	GLuint m_gBufVAO;
	GLuint m_shadowVAO;
	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_diffuseMap;
	GLuint m_specularMap;
	GLuint m_quadVao;
	GLuint m_quadVbo;
	GLuint m_shadingVAO;
	Cube*  m_cube;
};
