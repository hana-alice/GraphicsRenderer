#include "GeoObject.h"
#include "Singleton.h"
#include "GLWrapper.h"
#include <fstream>
#include <string>
#include "CommonFunc.h"

GeoObject::GeoObject()
{

}

GeoObject::~GeoObject()
{

}

void GeoObject::init()
{
    std::string srcPath = CommonFunc::getResourceDirectory();
    std::ifstream vsSource,fsSource,gsSource;

    vsSource.open(srcPath + "/resources/shader/common.vs");
    fsSource.open(srcPath + "/resources/shader/common.fs");
	gsSource.open(srcPath + "/resources/shader/geoshader.gs");
    
	std::string bufStr;
	std::string vs, fs, gs;
	while (getline(vsSource, bufStr))
	{
		vs += (bufStr + '\n');
	}
	while (getline(fsSource, bufStr))
	{
		fs += (bufStr + '\n');
	}
    while (getline(gsSource, bufStr))
	{
		gs += (bufStr + '\n');
	}
    m_glWrapper = Singleton::getInstance()->getGLWrapper();
    m_vertexShader = m_glWrapper->createShader(GL_VERTEX_SHADER,vs.c_str());
    m_fragmentShader = m_glWrapper->createShader(GL_FRAGMENT_SHADER,fs.c_str());
    m_geometryShader = m_glWrapper->createShader(GL_GEOMETRY_SHADER,gs.c_str());
    m_program = m_glWrapper->createProgram(m_vertexShader,m_fragmentShader,m_geometryShader);

    float vertices[] = {
        -0.5,0.5,0.0,
        0.5,0.5,0.0,
        0.5,-0.5,0.0,
        -0.5,-0.5,0.0
    };

    glGenVertexArrays(1,&m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1,&m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	GLint loc = glGetAttribLocation(m_program, "aPos");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);
	GLWrapper::errorCheck();
}

void GeoObject::render()
{
    glUseProgram(m_program);
    glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS,0,4);
    glBindVertexArray(0);
	GLWrapper::errorCheck();
    glUseProgram(0);
}

void GeoObject::destroy()
{
    
}