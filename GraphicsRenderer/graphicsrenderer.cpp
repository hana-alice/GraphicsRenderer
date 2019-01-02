#include "graphicsrenderer.h"
#include <QVBoxLayout>

GraphicsRenderer::GraphicsRenderer(QWidget *parent)
	: QMainWindow(parent), m_renderWidget(nullptr)
{
	ui.setupUi(this);
	ui.menuBar->hide();
	ui.mainToolBar->hide();
	ui.statusBar->hide();
	init();
}

void GraphicsRenderer::init()
{
	if (!m_renderWidget)
	{
		m_renderWidget = new GLRenderWidget(this);
	}
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(m_renderWidget);
	layout->setMargin(0);
	ui.centralWidget->setLayout(layout);
	this->grabKeyboard();
}

void GraphicsRenderer::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape & Qt::Key_Alt)
	{
		this->close();
	}
}
