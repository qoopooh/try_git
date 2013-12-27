#-------------------------------------------------
#
# Project created by QtCreator 2013-12-27T09:26:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qwid
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    resource.qrc

