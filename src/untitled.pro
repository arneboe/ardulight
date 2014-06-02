#-------------------------------------------------
#
# Project created by QtCreator 2013-12-25T21:13:13
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app

SOURCES += main.cpp \
    Global.cpp \
    Light.cpp \
    TrayMenu.cpp

HEADERS += protocol.h \
    LightController.h \
    TrayMenu.h \
    Light.h \
    ILight.h \
    Global.h \

HEADERS += d3d9.h
HEADERS += d3dx9.h
HEADERS += d3dx9d.h

RESOURCES += \
    resources.qrc

Debug:DESTDIR = ../debug
Debug:OBJECTS_DIR = ../debug/.obj
Debug:MOC_DIR = ../debug/.moc
Debug:RCC_DIR = ../debug/.rcc
Debug:UI_DIR = ../debug/.ui
