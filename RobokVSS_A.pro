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
    ball.cpp \
    fieldstate.cpp \
    robot.cpp \
    unit.cpp \
    vision.cpp \
    renderthread.cpp \
    utils.cpp \
    Interface/mainwindow.cpp \
    read_write.cpp

HEADERS  += \
    ball.h \
    fieldstate.h \
    robot.h \
    unit.h \
    vision.h \
    renderthread.h \
    floodfill.hpp \
    Interface/mainwindow.h \
    read_write.h

FORMS    += \
    Interface/mainwindow.ui

LIBS += `pkg-config opencv --libs`

DISTFILES += \
    README.md

RESOURCES += \
    Interface/robok.qrc
