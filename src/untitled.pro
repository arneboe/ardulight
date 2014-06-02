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
    Screen.cpp \
    Global.cpp \
    Light.cpp \
    TrayMenu.cpp

HEADERS += Screen.h \
    protocol.h \
    LightController.h \
    TrayMenu.h \
    Light.h \
    ILight.h \
    Global.h \

HEADERS += d3d9.h
HEADERS += d3dx9.h
HEADERS += d3dx9d.h

FORMS +=

win32:LIBS += -ld3d9 #-ld3dx9 -ld3dx9d
#win32:LIBS += -L"C:\\dx10sdk\\Lib\\x86" -ld3d9

RESOURCES += \
    resources.qrc
