/********************************************************************************
** Form generated from reading UI file 'graphicsrenderer.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHICSRENDERER_H
#define UI_GRAPHICSRENDERER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GraphicsRendererClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GraphicsRendererClass)
    {
        if (GraphicsRendererClass->objectName().isEmpty())
            GraphicsRendererClass->setObjectName(QStringLiteral("GraphicsRendererClass"));
        GraphicsRendererClass->resize(600, 400);
        menuBar = new QMenuBar(GraphicsRendererClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        GraphicsRendererClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GraphicsRendererClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        GraphicsRendererClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(GraphicsRendererClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        GraphicsRendererClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(GraphicsRendererClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        GraphicsRendererClass->setStatusBar(statusBar);

        retranslateUi(GraphicsRendererClass);

        QMetaObject::connectSlotsByName(GraphicsRendererClass);
    } // setupUi

    void retranslateUi(QMainWindow *GraphicsRendererClass)
    {
        GraphicsRendererClass->setWindowTitle(QApplication::translate("GraphicsRendererClass", "GraphicsRenderer", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GraphicsRendererClass: public Ui_GraphicsRendererClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHICSRENDERER_H
