#-------------------------------------------------
#
# Project created by QtCreator 2013-12-25T21:13:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app


SOURCES += *.cpp

HEADERS += *.h
HEADERS += d3d9.h
HEADERS += d3dx9.h
HEADERS += d3dx9d.h

FORMS += mainwindow.ui

win32:LIBS += -L"C:\\dx10sdk\\Lib\\x86" -ld3d9 #-ld3dx9 -ld3dx9d

INCLUDEPATH += C:\dx10sdk\include
