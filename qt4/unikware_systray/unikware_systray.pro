#-------------------------------------------------
#
# Project created by QtCreator 2014-02-13T17:37:50
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = unikware_systray
TEMPLATE = app


SOURCES += main.cpp\
        unikware.cpp \
    database.cpp

HEADERS  += unikware.h \
    database.h

RESOURCES += \
    unik.qrc


