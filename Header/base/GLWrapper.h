#pragma once
#include <functional>
#include <vector>
#include "glad/glad.h"

typedef std::function<void(void)> RenderFunc;
typedef std::function<void(void)> InitFunc;
typedef std::function<void(void)> preRenderFunc;
typedef std::function<void(void)> afterRenderFunc;
typedef std::function<void(void)> DestroyFunc;
class GLWrapper
{
public:
    GLWrapper();
    ~GLWrapper();

    static void errorCheck();

    void registerInitFunc(InitFunc func);
    template <typename T,typename F>
    void registerInitFunc(T* t, F f)
    {
        registerInitFunc(std::bind(f,t));
    }

    void registerRenderFunc(RenderFunc func);
    template <typename T,typename F>
    void registerRenderFunc(T* t, F f)
    {
        registerRenderFunc(std::bind(f, t));
    }

    //void registerPreFuncRender(preRenderFunc func);
    
    void registerDestroyFunc(DestroyFunc func);
    template<typename T,typename F>
    void registerDestroyFunc(T* t, F f)
    {
        registerDestroyFunc(std::bind(f,t));
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
    std::vector<DestroyFunc> m_destroyFuncVec;
};
