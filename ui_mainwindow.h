/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
<<<<<<< HEAD
** Created: Sun Dec 16 16:00:00 2012
=======
** Created: Sun Dec 16 16:22:44 2012
>>>>>>> c2633db219b102e4217f5c1f53afe195f3264cee
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include <glwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_Model;
    QWidget *center;
    QGridLayout *gridLayout;
    GLWidget *glWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_2;
    QWidget *btnBar;
    QCheckBox *rotateBtn;
    QCheckBox *vboBtn;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(922, 636);
        actionLoad_Model = new QAction(MainWindow);
        actionLoad_Model->setObjectName(QString::fromUtf8("actionLoad_Model"));
        center = new QWidget(MainWindow);
        center->setObjectName(QString::fromUtf8("center"));
        gridLayout = new QGridLayout(center);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        glWidget = new GLWidget(center);
        glWidget->setObjectName(QString::fromUtf8("glWidget"));

        gridLayout->addWidget(glWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(center);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 922, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(148, 53));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout_2 = new QGridLayout(dockWidgetContents);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        btnBar = new QWidget(dockWidgetContents);
        btnBar->setObjectName(QString::fromUtf8("btnBar"));
        btnBar->setMinimumSize(QSize(130, 0));
        rotateBtn = new QCheckBox(btnBar);
        rotateBtn->setObjectName(QString::fromUtf8("rotateBtn"));
        rotateBtn->setGeometry(QRect(0, 10, 131, 22));
        rotateBtn->setChecked(true);
        vboBtn = new QCheckBox(btnBar);
        vboBtn->setObjectName(QString::fromUtf8("vboBtn"));
        vboBtn->setGeometry(QRect(0, 40, 108, 22));

        gridLayout_2->addWidget(btnBar, 0, 0, 1, 1);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionLoad_Model);

        retranslateUi(MainWindow);
        QObject::connect(actionLoad_Model, SIGNAL(triggered()), glWidget, SLOT(fileOpen()));
        QObject::connect(rotateBtn, SIGNAL(clicked()), glWidget, SLOT(toggleRotate()));
        QObject::connect(vboBtn, SIGNAL(clicked()), glWidget, SLOT(toggleVboDraw()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionLoad_Model->setText(QApplication::translate("MainWindow", "Load Model", 0, QApplication::UnicodeUTF8));
        actionLoad_Model->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        rotateBtn->setText(QApplication::translate("MainWindow", "Rotate Camera", 0, QApplication::UnicodeUTF8));
        vboBtn->setText(QApplication::translate("MainWindow", "VBO draw", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
