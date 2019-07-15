#pragma once
#include "glad\glad.h"
#include "Singleton.h"

class Cube
{
public:
    Cube(/* args */);
    ~Cube();

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
};
