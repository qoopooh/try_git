#-------------------------------------------------
#
# Project created by QtCreator 2014-01-06T16:41:08
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network

TARGET = qscratch
TEMPLATE = app

SOURCES += main.cpp \
    chat.cpp \
    gatewaymessage.cpp \
    encrypt.cpp

HEADERS += \
    chat.h \
    gatewaymessage.h \
    encrypt.h
