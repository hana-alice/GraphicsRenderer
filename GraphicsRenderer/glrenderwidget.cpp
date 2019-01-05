#include "glrenderwidget.h"
#include "GLES3/gl3.h"
#include <iostream>
#include <QImage>

#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/include/stb_image.h"

#define GLCheck(x) \
(x);\
{\
	if (glGetError() != GL_NO_ERROR)\
	{\
		for (;;)\
		{\
		}\
	}\
};

GLfloat vertices[] = {
	-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,  0.0,0.5,
	 0.5, -0.5, 0.0, 1.0, 1.0, 0.0, 1.0,  0.0,0.0,
	 0.5,  0.5, 0.0, 1.0, 0.0, 1.0, 1.0,  0.0,0.0,
	-0.5,  0.5, 0.0, 1.0, 1.0, 1.0, 1.0,  0.0,0.0
};


unsigned short indices[] = {
	0,1,2,
	0,2,3
};

GLRenderWidget::GLRenderWidget(QWidget *parent)
	: QOpenGLWidget(parent), m_pgmObj(nullptr)
{
	ui.setupUi(this);
}

GLRenderWidget::~GLRenderWidget()
{
	
}

void GLRenderWidget::initializeGL()
{
	initializeOpenGLFunctions();
	if (!m_pgmObj)
	{
		m_pgmObj = new QOpenGLShaderProgram(this);
	}
	if (!m_pgmObj->addShaderFromSourceFile(QOpenGLShader::Vertex, "vertexShader.vs"))
	{
		return;
	}
	if (!m_pgmObj->addShaderFromSourceFile(QOpenGLShader::Fragment,"fragShader.fs"))
	{
		return;
	}
	if (!m_pgmObj->link())
	{
		return;
	}
	if (!m_pgmObj->bind())
	{
		return;
	}
	m_pgmid = m_pgmObj->programId();
	GLCheck(glUseProgram(m_pgmid));
	m_matrixLoc = glGetUniformLocation(m_pgmid, "matrix");
	m_vertexLoc = glGetAttribLocation(m_pgmid, "inPos");
	m_colorLoc = glGetAttribLocation(m_pgmid, "inColor");
	GLCheck(glGenBuffers(1, &m_vbo));
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
	GLCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));	
	GLCheck(glVertexAttribPointer(m_vertexLoc, 3, GL_FLOAT, false, 9 * sizeof(GL_FLOAT), 0));
	GLCheck(glVertexAttribPointer(m_colorLoc, 4, GL_FLOAT, false, 9 * sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT))));
	GLCheck(glEnableVertexAttribArray(m_vertexLoc));
	GLCheck(glEnableVertexAttribArray(m_colorLoc));
	//GLCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCheck(glGenBuffers(1, &m_ibo));
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));
	GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	//GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	int width, height, nrChannels;
	unsigned char *data = stbi_load("Resources/wood.png", &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cout << "gg\n";
	}
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	GLuint texloc = glGetAttribLocation(m_pgmid, "inTexCoord");
	glVertexAttribPointer(texloc, 2, GL_FLOAT, false, 9 * sizeof(GL_FLOAT), (void*)(7 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(texloc);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void GLRenderWidget::paintGL()
{
	GLCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCheck(glClearColor(0, 0.5, 0.5, 1));
	
	GLCheck(glUseProgram(m_pgmid));
	GLCheck(glBindTexture(GL_TEXTURE_2D, m_tex));
	
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));
	GLenum err = glGetError();
	
	(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0));
	if (err != GL_NO_ERROR)
	{
		std::cout << "gg\n";
	}
	
}

void GLRenderWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}
