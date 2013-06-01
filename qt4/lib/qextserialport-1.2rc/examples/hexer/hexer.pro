#-------------------------------------------------
#
# Project created by QtCreator 2013-06-01T08:37:41
#
#-------------------------------------------------

QT       += core gui
contains(QT_VERSION, ^5\\..*\\..*): QT += widgets

TARGET = Hexer
TEMPLATE = app
CONFIG += debug

include(../../src/qextserialport.pri)

SOURCES += main.cpp\
        dialog.cpp\
        hled.cpp

HEADERS  += dialog.h \
            hled.h

FORMS    += dialog.ui

debug {
  CONFIG += console
}

