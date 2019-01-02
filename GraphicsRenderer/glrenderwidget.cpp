#include "glrenderwidget.h"
#include "GLES3/gl3.h"
#include <iostream>

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
	-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,
	 0.5, -0.5, 0.0, 1.0, 1.0, 0.0, 1.0,
	 0.5,  0.5, 0.0, 1.0, 0.0, 1.0, 1.0,
	-0.5,  0.5, 0.0, 1.0, 1.0, 1.0, 1.0
};

unsigned short indices[] = {
	0,1,2,
	0,2,3
};

GLRenderWidget::GLRenderWidget(QWidget *parent)
	: QOpenGLWidget(parent), m_pgm(nullptr)
{
	ui.setupUi(this);
}

GLRenderWidget::~GLRenderWidget()
{
	
}

void GLRenderWidget::initializeGL()
{
	initializeOpenGLFunctions();
	if (!m_pgm)
	{
		m_pgm = new QOpenGLShaderProgram(this);
	}
	if (!m_pgm->addShaderFromSourceFile(QOpenGLShader::Vertex, "vertexShader.vs"))
	{
		return;
	}
	if (!m_pgm->addShaderFromSourceFile(QOpenGLShader::Fragment,"fragShader.fs"))
	{
		return;
	}
	if (!m_pgm->link())
	{
		return;
	}
	if (!m_pgm->bind())
	{
		return;
	}

	m_pgmid = m_pgm->programId();
	GLCheck(glUseProgram(m_pgmid));
	//m_matrixLoc = glGetUniformLocation(m_pgmid, "matrix");
	m_vertexLoc = glGetAttribLocation(m_pgmid, "inPos");
	m_colorLoc = glGetAttribLocation(m_pgmid, "inColor");
	GLCheck(glGenBuffers(1, &m_vbo));
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
	GLCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));	
	GLCheck(glVertexAttribPointer(m_vertexLoc, 3, GL_FLOAT, false, 7 * sizeof(GL_FLOAT), 0));
	GLCheck(glVertexAttribPointer(m_colorLoc, 4, GL_FLOAT, false, 7 * sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT))));
	GLCheck(glEnableVertexAttribArray(m_vertexLoc));
	GLCheck(glEnableVertexAttribArray(m_colorLoc));
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCheck(glGenBuffers(1, &m_ibo));
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));
	GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

}

void GLRenderWidget::paintGL()
{
	GLCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCheck(glClearColor(0, 0.5, 0.5, 1));
	GLCheck(glUseProgram(m_pgmid));
	GLCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
	GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));
	(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0));
	
}

void GLRenderWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}
