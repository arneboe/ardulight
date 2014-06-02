#-------------------------------------------------
#
# Project created by QtCreator 2014-06-02T17:43:53
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AmbilightPlugin
TEMPLATE = lib

SOURCES += DesktopController.cpp \
           Screen.cpp

HEADERS += DesktopController.h \
           Screen.h

INCLUDEPATH += ../../src

QMAKE_CXXFLAGS += -std=c++11

#FIXME need alternative for linux
win32:LIBS += -ld3d9 #-ld3dx9 -ld3dx9d

Debug:DESTDIR = ../../debug/plugins
Debug:OBJECTS_DIR = ../../debug/.obj
Debug:MOC_DIR = ../../debug/.moc
Debug:RCC_DIR = ../../debug/.rcc
Debug:UI_DIR = ../../debug/.ui
