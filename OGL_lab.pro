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
    camera.cpp \
    resourceloader.cpp \
    glm.cpp \
    lib/targa.cpp \
    particles/particleemitter.cpp \
    ShapesSphere.cpp \
    Shape.cpp \
    Vertex3f.cpp \
    particles/rain.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    vector.h \
    camera.h \
    obj.h \
    scene.h \
    resourceloader.h \
    glm.h \
    lib/targa.h \
    particles/particleemitter.h \
    common.h \
    ShapesSphere.h \
    Shape.h \
    Vertex3f.h \
    particles/rain.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    shaders/toon.frag \
    shaders/toon.vert \
    shaders/edge_detect.frag \
    shaders/edge_enhance.frag \
    shaders/combine.frag
