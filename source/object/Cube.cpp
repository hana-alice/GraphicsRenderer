#include "Cube.h"
#include "GLWrapper.h"
#include <string>
#include "CommonFunc.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stbimg/stb_image.h"

static const char* vertexShader = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"void main()"
"{"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
"TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);"
"}";

static const char* fragShader = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"
"uniform sampler2D texture2;\n"
"void main()"
"{"
"FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);"
"}";

Cube::Cube(/* args */)
    :m_glWrapper(nullptr)
{
}

Cube::~Cube()
{
    glDetachShader(m_program, m_vertexShader);
	glDetachShader(m_program, m_fragmentShader);
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteProgram(m_program);
}

void Cube::init()
{
    m_glWrapper = Singleton::getInstance()->getGLWrapper();
    m_vertexShader = m_glWrapper->createShader(GL_VERTEX_SHADER,vertexShader);
    m_fragmentShader = m_glWrapper->createShader(GL_FRAGMENT_SHADER,fragShader);
    m_program = m_glWrapper->createProgram(m_vertexShader,m_fragmentShader);

    float vertices[] = 
    {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f
    };

    unsigned short indices[] = 
    {
        0,1,3,  3,1,2,  
        2,1,5,  2,5,6,
        5,4,6,  6,4,7,
        7,4,3,  3,4,0,
        3,2,7,  7,2,6,
        0,4,1,  1,4,5
    };

    glGenVertexArrays(1,&m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1,&m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    glGenBuffers(1,&m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
	GLWrapper::errorCheck();
    glUseProgram(m_program);
    GLint posLoc = glGetAttribLocation(m_program,"aPos");
    glVertexAttribPointer(posLoc,3,GL_FLOAT,GL_FALSE,5*sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(posLoc);

    GLint texLoc = glGetAttribLocation(m_program,"aTexCoord");
    glVertexAttribPointer(texLoc,2,GL_FLOAT,GL_FALSE,5*sizeof(GL_FLOAT),(void*)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(texLoc);
	GLWrapper::errorCheck();
    unsigned int texture1, texture2;
    glGenTextures(1,&texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture1);
	GLuint samplerPos1 = glGetUniformLocation(m_program, "texture1");
	GLWrapper::errorCheck();
    glUniform1i(samplerPos1,0);
	GLWrapper::errorCheck();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	GLWrapper::errorCheck();
	std::string path = CommonFunc::getResourceDirectory();
    int width, height, channels;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load((path + "/resources/jojo0.jpg").c_str(),&width,&height,&channels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        //TODO: exception or debug info here
    }
    stbi_image_free(data);
	GLWrapper::errorCheck();
    glGenTextures(1,&texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,texture2);
    glUniform1i(glGetUniformLocation(m_program,"texture2"),1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    data = stbi_load((path + "/resources/dio.jpg").c_str(),&width,&height,&channels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        //TODO: exception or debug info here
    }
    stbi_image_free(data);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Cube::render()
{
    glViewport(0, 0, 1280, 720);
	glUseProgram(m_program);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

    glBindVertexArray(m_vao);
    GLWrapper::errorCheck();
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,NULL);
    GLWrapper::errorCheck();

    glBindVertexArray(0);
    glUseProgram(0);
}

void Cube::destroy()
{
    delete this;
}