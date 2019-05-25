#pragma once

class GLWrapper;

class Singleton
{
public:
    Singleton();
    ~Singleton();

    void setGLWrapper(GLWrapper* wrapper);

    GLWrapper* getGLWrapper();

private:
    GLWrapper* m_glWrapper;
}