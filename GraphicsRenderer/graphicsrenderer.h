#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_graphicsrenderer.h"
#include "glrenderwidget.h"
#include <QKeyevent>

class GraphicsRenderer : public QMainWindow
{
	Q_OBJECT

public:
	GraphicsRenderer(QWidget *parent = Q_NULLPTR);

	void init();

private:
	virtual void keyPressEvent(QKeyEvent *event);

private:          
	Ui::GraphicsRendererClass ui;

	GLRenderWidget* m_renderWidget;
};
