#-------------------------------------------------
#
# Project created by QtCreator 2014-02-20T17:46:35
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = sounder
TEMPLATE = app

include(spectrum.pri)


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

fftreal_dir = ../lib/fftreal

INCLUDEPATH += $${fftreal_dir}

OTHER_FILES +=
