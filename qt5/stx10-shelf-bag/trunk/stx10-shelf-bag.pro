#-------------------------------------------------
#
# Project created by QtCreator 2011-11-30T13:33:33
#
#-------------------------------------------------

QT       += core gui serialport widgets

TARGET = STx10-shelf-and-bag
TEMPLATE = app

HEADERS  += mainwindow.h \
    aaecommand.h \
    log.h \
    streader.h \
    bootloader.h \
    aaereader.h \
    XonXoff.h

SOURCES += main.cpp\
    mainwindow.cpp \
    aaecommand.cpp \
    log.cpp \
    streader.cpp \
    bootloader.cpp \
    aaereader.cpp

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc

