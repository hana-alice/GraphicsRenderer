#pragma once

#include <QWidget>
#include <QOPenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "ui_glrenderwidget.h"
#include <QOpenGLVertexArrayObject> 

class GLRenderWidget : public QOpenGLWidget , protected QOpenGLFunctions
      {
	Q_OBJECT

public:
	explicit GLRenderWidget(QWidget *parent = Q_NULLPTR);
	~GLRenderWidget();

protected:
	void initializeGL() override;

	void paintGL() override;

	void resizeGL(int w,int h) override;

private:
	QOpenGLShaderProgram* m_pgmObj;
	Ui::GLRenderWidget ui;

	GLint m_pgmid;
	GLint m_matrixLoc;
	GLint m_vertexLoc;
	GLint m_colorLoc;

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_tex;
};
