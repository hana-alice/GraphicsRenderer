#pragma once

#include "Singleton.h"
#include "glad/glad.h"

class PBRDemo
{
public:
	PBRDemo();
	~PBRDemo();

	void init();

	void render();

	void destroy();

private:
	struct PBRParams
	{
		uint32_t albedo;
		uint32_t normal;
		uint32_t metallic;
		uint32_t roughness;
		uint32_t ao;
	};

	unsigned int loadTexture(const char* path, bool hdr = false);

	void prepareCube();

	void prepareVerts();

	GLWrapper* m_glWrapper;
	GLuint m_vertexShader;
	GLuint m_fragmentShader;
	GLuint m_program;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;

	GLuint m_cubePgm;
	GLuint m_skyboxPgm;
	GLuint m_hdrTex;
	GLuint m_envCubeTex;

    uint32_t 	m_indexCount;
	PBRParams 	m_pbrParams;
};