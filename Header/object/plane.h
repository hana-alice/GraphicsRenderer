#pragma once
#include "glad\glad.h"

class GLWrapper;
class Plane
{
public:
    Plane();
    ~Plane();

    void init();

	void render();

	void destroy();
private:
    GLWrapper* m_glWrapper;
	GLuint m_vertexShader;
	GLuint m_fragmentShader;
	GLuint m_program;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_tex;

	GLuint m_shadowVao;
};
