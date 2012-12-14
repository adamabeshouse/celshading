#-------------------------------------------------
#
# Project created by QtCreator 2012-06-25T21:00:29
#
#-------------------------------------------------


QT       += core gui
QT       += opengl

TARGET = OGL_lab
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    obj.cpp \
    camera.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    vector.h \
    camera.h \
    obj.h \
    scene.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    shaders/toon.frag \
    shaders/toon.vert
