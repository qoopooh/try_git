#-------------------------------------------------
#
# Project created by QtCreator 2011-11-30T13:33:33
#
#-------------------------------------------------

QT       += core gui widgets
lessThan(QT_MAJOR_VERSION, 5) {
  CONFIG += serialport
  LIBS += -lqtserialport
} else {
  QT += serialport
}

TARGET = Prx10Flash
TEMPLATE = app

HEADERS  += mainwindow.h \
    prreader.h \
    aaecommand.h \
    log.h \
    streader.h \
    bootloader.h \
    aaereader.h \
    XonXoff.h

SOURCES += main.cpp\
    mainwindow.cpp \
    prreader.cpp \
    aaecommand.cpp \
    log.cpp \
    streader.cpp \
    bootloader.cpp \
    aaereader.cpp

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc

