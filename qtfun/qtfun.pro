#-------------------------------------------------
#
# Project created by QtCreator 2017-04-13T17:39:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CFLAGS   = -std=c99 -static
QMAKE_CXXFLAGS = -std=c++0x -static
#QMAKE_LFLAGS = -static

TARGET = qtfun
TEMPLATE = app

#LIBS += -L /usr/local/arm/tslib/install

SOURCES += main.cpp\
        dialog.cpp \
    Camera.cpp

HEADERS  += dialog.h \
    Camera.hpp \
    SimpleBuffer.hpp \
    Frame.hpp \
    utils.h

FORMS    += dialog.ui
