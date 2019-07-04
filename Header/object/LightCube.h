#pragma once
#include "glad\glad.h"

class GLWrapper;
class LightCube
{
public:
	enum CUBE_TYPE
	{
		OBJECT,
		LIGHT
	};

    LightCube(CUBE_TYPE tp);
    ~LightCube();

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
    CUBE_TYPE   m_type;
};
