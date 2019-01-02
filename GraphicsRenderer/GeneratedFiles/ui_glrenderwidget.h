/********************************************************************************
** Form generated from reading UI file 'glrenderwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GLRENDERWIDGET_H
#define UI_GLRENDERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GLRenderWidget
{
public:

    void setupUi(QWidget *GLRenderWidget)
    {
        if (GLRenderWidget->objectName().isEmpty())
            GLRenderWidget->setObjectName(QStringLiteral("GLRenderWidget"));
        GLRenderWidget->resize(400, 300);

        retranslateUi(GLRenderWidget);

        QMetaObject::connectSlotsByName(GLRenderWidget);
    } // setupUi

    void retranslateUi(QWidget *GLRenderWidget)
    {
        GLRenderWidget->setWindowTitle(QApplication::translate("GLRenderWidget", "GLRenderWidget", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GLRenderWidget: public Ui_GLRenderWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GLRENDERWIDGET_H
