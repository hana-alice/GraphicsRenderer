#pragma once
#include <functional>
#include <vector>
#include "glad/glad.h"

typedef void (*InitFunc)();
typedef void (*RenderFunc)();
class GLWrapper
{
public:
    GLWrapper();
    ~GLWrapper();

    static void errorCheck();

    bool registerInitFunc(InitFunc func);
    template <typename T,typename F>
    bool registerInitFunc(T* t, F* f)
    {
        registerInitFunc(std::bind(f,t));
    }

    bool registerRenderFunc(RenderFunc funxc);
    template <typename T,typename F>
    bool registerRenderFunc(T* t, F* f)
    {
        registerRenderFunc(f,t);
    }

    void init();

    void render();

    //TODO:
    GLuint createShader(int type, const char* shaderSrc);
    
    GLuint createProgram(int vertexShader, int fragmentShader);
    //destroyShader();
    //destroyProgram();
private:
    std::vector<InitFunc> m_initFuncVec;
    std::vector<RenderFunc> m_renderFuncVec;
};
