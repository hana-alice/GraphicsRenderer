#pragma once
#include "glfw/glfw3.h"
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
};
