#-------------------------------------------------
#
# Project created by QtCreator 2016-12-03T11:35:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MeshViewer
TEMPLATE = app

LIBS += opengl32.lib

SOURCES += main.cpp\
        mainwindow.cpp \
    helpdialog.cpp \
    glwidget.cpp \
    transform3d.cpp \
    camera3d.cpp \
    fpscounter.cpp \
    inputmanager.cpp \
    debugmanager.cpp \
    mesh.cpp \
    texturemanager.cpp \
    material.cpp \
    light.cpp \
    materialmanager.cpp \
    entity3d.cpp \
    skybox.cpp \
    entitymanager.cpp \
    lightmanager.cpp \
    node3d.cpp \
    nodemanager.cpp \
    shadermanager.cpp \
    framebuffer.cpp \
    renderoptions.cpp

HEADERS  += mainwindow.h \
    helpdialog.h \
    glwidget.h \
    vertex.h \
    transform3d.h \
    camera3d.h \
    fpscounter.h \
    inputmanager.h \
    debugmanager.h \
    mesh.h \
    texturemanager.h \
    material.h \
    light.h \
    materialmanager.h \
    entity3d.h \
    skybox.h \
    entitymanager.h \
    lightmanager.h \
    node3d.h \
    nodemanager.h \
    global.h \
    shadermanager.h \
    framebuffer.h \
    renderoptions.h

FORMS    += mainwindow.ui \
    helpdialog.ui \
    renderoptions.ui

RESOURCES += \
    resources.qrc

DISTFILES +=
