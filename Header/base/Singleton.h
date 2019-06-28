#pragma once

class GLWrapper;

class Singleton
{
public:
    Singleton();
    ~Singleton();

    static Singleton* getInstance(){return s_instance;};

    void setGLWrapper(GLWrapper* wrapper);

    GLWrapper* getGLWrapper();

private:
    static Singleton* s_instance;
    GLWrapper* m_glWrapper;
}