#-------------------------------------------------
#
# Project created by Berm 2013-05-22T14:47:33
#
#-------------------------------------------------

QT       += core gui
include(../../lib/qextserialport-1.2rc/src/qextserialport.pri)
lessThan(QT_MAJOR_VERSION, 5) {
  CONFIG += qextserialport
} else {
  QT += serialport widgets
}
TARGET = STx10-shelf-and-bag
TEMPLATE = app

HEADERS  += mainwindow.h \
    aaecommand.h \
    log.h \
    streader.h \
    bootloader.h \
    aaereader.h \
    XonXoff.h \
    epctreeitem.h \
    epctreemodel.h

SOURCES += main.cpp\
    mainwindow.cpp \
    aaecommand.cpp \
    log.cpp \
    streader.cpp \
    bootloader.cpp \
    aaereader.cpp \
    epctreeitem.cpp \
    epctreemodel.cpp

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc





