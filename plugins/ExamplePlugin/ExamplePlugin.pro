#-------------------------------------------------
#
# Project created by QtCreator 2014-06-02T15:59:35
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExamplePlugin
TEMPLATE = lib

SOURCES += ExampleController.cpp

HEADERS += ExampleController.h

INCLUDEPATH += ../../src

QMAKE_CXXFLAGS += -std=c++11



Debug:DESTDIR = ../../debug/plugins
Debug:OBJECTS_DIR = ../../debug/.obj
Debug:MOC_DIR = ../../debug/.moc
Debug:RCC_DIR = ../../debug/.rcc
Debug:UI_DIR = ../../debug/.ui
