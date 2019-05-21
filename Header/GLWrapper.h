#pragma once
#include <functional>
#include <vector>

typedef void (*InitFunc)();
typedef void (*RenderFunc)();
class GLWrapper
{
public:
    GLWrapper();
    ~GLWrapper();

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
    // createShader();
    //createProgram();
    //destroyShader();
    //destroyProgram();
private:
    std::vector<InitFunc> m_initFuncVec;
    std::vector<RenderFunc> m_renderFuncVec;
};
