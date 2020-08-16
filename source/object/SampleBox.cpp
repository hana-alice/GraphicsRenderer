#include "SampleBox.h"
#include "Singleton.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLWrapper.h"

static const char* vertexShader =
	"#version 100\n"
	"precision mediump float;\n"
	"attribute vec3 aPos;\n"
	"uniform vec4 inColor;\n"
	"varying vec4 aColor;\n"

	"uniform mat4 modelMat;\n"
	"uniform mat4 viewMat;\n"
	"uniform mat4 projectionMat;\n"
	"void main()\n"
	"{"
	"gl_Position = projectionMat * viewMat * modelMat *vec4(aPos,1.0);"
    //"gl_Position = vec4(aPos,1.0);"
	"aColor = inColor;"
	"}";

static const char* fragmentShader =
	"#version 100\n"
	"precision mediump float;\n"
	"varying vec4 aColor;"
	"void main()"
	"{"
	"gl_FragColor = aColor;"
	"}";

SampleBox::SampleBox()
{

}

SampleBox::~SampleBox()
{
    glDetachShader(m_program, m_vertexShader);
	glDetachShader(m_program, m_fragmentShader);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteProgram(m_program);
}

void SampleBox::init()
{
    m_glWrapper = Singleton::getInstance()->getGLWrapper();
	m_vertexShader = m_glWrapper->createShader(GL_VERTEX_SHADER, vertexShader);
	m_fragmentShader = m_glWrapper->createShader(GL_FRAGMENT_SHADER, fragmentShader);
	m_program = m_glWrapper->createProgram(m_vertexShader, m_fragmentShader);

	float vertex[] = {
		-0.15, 0.5, -0.0,
		0.15,  0.5, -0.0,
		0.15, -0.5, -0.0,
		-0.15,-0.5, -0.0
	};
	unsigned short index[] = {
		0,3,1,
		1,3,2
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	int size = sizeof(vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	GLWrapper::errorCheck();
	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	glUseProgram(m_program);
	GLint posLoc = 0, colorLoc = 0, modelLoc = 0, viewLoc = 0, projectionLoc = 0;
	posLoc = glGetAttribLocation(m_program, "aPos");
	glEnableVertexAttribArray(posLoc);
	GLWrapper::errorCheck();
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	GLWrapper::errorCheck();
	colorLoc = glGetUniformLocation(m_program, "inColor");
	GLWrapper::errorCheck();
	glUniform4f(colorLoc, 1.0, 0.0, 0.0, 1.0);
	GLWrapper::errorCheck();
	glEnableVertexAttribArray(colorLoc);
	GLWrapper::errorCheck();
	glm::mat4 model = glm::mat4(1.0f);

	float arr[9] = {
		0.321152,
		0.947028,
		0.000000,
		-0.947028,
		0.321152,
		-0.000000,
		-0.000000,
		0.000000,
		1.000000
	};
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			model[i][j] = arr[i * 3 + j];
		}
	}
	glm::mat4 transMat;
	transMat = glm::translate(transMat, glm::vec3(0.0f, 0.7f, 0.0f));
	glm::mat4 ratioMat = glm::scale(model, glm::vec3(1280.0f, 720.0f, 1.0f));
	model = glm::inverse(ratioMat) * glm::inverse(model) * ratioMat * transMat;
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
	glm::mat4 tempMat(1.0f);
	glm::mat4 rr = glm::rotate(tempMat, glm::radians(23.74f), glm::vec3(0.0f, 0.0f, 1.0f));
	//view = Singleton::getInstance()->getViewMat();
	//proj = glm::perspective(glm::radians(Singleton::getInstance()->getFOV()), (float)(1280.0 / 720.0), 0.1f, 100.0f);
	modelLoc = glGetUniformLocation(m_program, "modelMat");
	viewLoc = glGetUniformLocation(m_program, "viewMat");
	projectionLoc = glGetUniformLocation(m_program, "projectionMat");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &proj[0][0]);
	glUseProgram(0);
	glBindVertexArray(0);

	GLWrapper::errorCheck();
}

void SampleBox::render()
{
    glViewport(0, 0, 1280, 720);
	glUseProgram(m_program);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(m_vao);
	GLWrapper::errorCheck();

	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, NULL);
	GLWrapper::errorCheck();

	glBindVertexArray(0);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(0);
	GLWrapper::errorCheck();
}

void SampleBox::destroy()
{
	delete this;
}