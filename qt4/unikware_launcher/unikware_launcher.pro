#-------------------------------------------------
#
# Project created by QtCreator 2014-02-19T11:23:10
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = unikware_launcher
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

SOURCES += main.cpp \
    unikprocess.cpp

win32 {
    RC_FILE = unik.rc # icon on exe file
}

HEADERS += \
    unikprocess.h
