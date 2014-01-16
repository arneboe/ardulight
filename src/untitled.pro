#-------------------------------------------------
#
# Project created by QtCreator 2013-12-25T21:13:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           DirectX.cpp

HEADERS += mainwindow.h \
           DirectX.h
HEADERS += d3d9.h
HEADERS += d3dx9.h
HEADERS += d3dx9d.h

FORMS += mainwindow.ui

win32:LIBS += -ld3d9 #-ld3dx9 -ld3dx9d
#win32:LIBS += -L"C:\\dx10sdk\\Lib\\x86" -ld3d9
