#pragma once
#include "GLWrapper.h"

class Widget
{
public:
    Widget(/* args */);
    ~Widget();

    void init();

    void show();

protected:
	void initContext();

	void initObject();

    void render();
private:
    GLWrapper*  m_glWrapper;
    int         m_width;
    int         m_height;
    bool        m_initialised;
};

