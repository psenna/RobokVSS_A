#-------------------------------------------------
#
# Project created by QtCreator 2015-03-02T20:35:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RobokVSS_A
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ball.cpp \
    fieldstate.cpp \
    robot.cpp \
    structs.cpp \
    unit.cpp \
    vision.cpp

HEADERS  += mainwindow.h \
    ball.h \
    fieldstate.h \
    robot.h \
    unit.h \
    encontraObjeto.hpp \
    vision.h

FORMS    += mainwindow.ui

LIBS += `pkg-config opencv --libs`

DISTFILES += \
    README.md
