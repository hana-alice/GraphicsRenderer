#pragma once
#include "glad/glad.h"
#include "Singleton.h"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

class Cube
{
public:
    Cube(/* args */);
    ~Cube();

    void init();

	void render();

	void destroy();

	void setModelMatrix(glm::mat4 modelMat);

	void setColor(glm::vec4 color);
private:
    GLWrapper* m_glWrapper;
	GLuint m_vertexShader;
	GLuint m_fragmentShader;
	GLuint m_program;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_tex;
	glm::mat4 m_modelMat;
	glm::vec4 m_color;
};
