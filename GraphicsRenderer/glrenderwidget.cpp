#include "glrenderwidget.h"
#include "GLES3/gl3.h"

GLfloat vertices[] = {
	-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,
	 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0,
	 0.5,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0,
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
	m_matrixLoc = glGetUniformLocation(m_pgmid, "matrix");
	m_vertexLoc = glGetUniformLocation(m_pgmid, "pos");
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint loc = 0;
	loc = glGetAttribLocation(m_pgmid, "inPos");
	glVertexAttribPointer(loc,sizeof(GL_FLOAT)*7);

}

void GLRenderWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0.5, 0.5, 1);
	
}

void GLRenderWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}
