#-------------------------------------------------
#
# Project created by QtCreator 2013-02-04T11:22:31
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = keyboardCommand
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ardrone.cpp

HEADERS  += mainwindow.h \
    ardrone.h

FORMS    += mainwindow.ui
