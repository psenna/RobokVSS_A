#-------------------------------------------------
#
# Project created by QtCreator 2015-03-02T20:35:50
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RobokVSS_A
TEMPLATE = app


SOURCES += main.cpp\
    ball.cpp \
    fieldstate.cpp \
    robot.cpp \
    unit.cpp \
    utils.cpp \    
    read_write.cpp \
    Interface/mainwindow.cpp \
    Vision/renderthread.cpp \
    Vision/vision.cpp \
    Communitacion/serial.cpp \    
    Strategy/default.cpp \
    motion.cpp \
    Strategy/doubleattack.cpp

HEADERS  += \
    ball.h \
    fieldstate.h \
    robot.h \
    unit.h \    
    read_write.h \
    utils.h \
    Interface/mainwindow.h \
    Vision/floodfill.hpp \
    Vision/renderthread.h \
    Vision/vision.h \
    Communitacion/serial.h \    
    Strategy/default.h \
    Strategy/strategy.h \
    motion.h \
    Strategy/doubleattack.h

FORMS    += \
    Interface/mainwindow.ui

LIBS += `pkg-config opencv --libs`

DISTFILES += \
    README.md

RESOURCES += \
    Interface/robok.qrc
