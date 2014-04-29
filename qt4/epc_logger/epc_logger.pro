#-------------------------------------------------
#
# Project created by Berm 2013-05-22T14:47:33
#
#-------------------------------------------------

QT       += core gui sql
include(../lib/qextserialport-1.2rc/src/qextserialport.pri)
CONFIG += qextserialport
lessThan(QT_MAJOR_VERSION, 5) {
} else {
  QT += widgets
}
TARGET = TireTagLogger
TEMPLATE = app

HEADERS  += mainwindow.h \
    aaecommand.h \
    log.h \
    streader.h \
    aaereader.h \
    epctreeitem.h \
    epctreemodel.h \
    epcdb.h

SOURCES += main.cpp\
    mainwindow.cpp \
    aaecommand.cpp \
    log.cpp \
    streader.cpp \
    aaereader.cpp \
    epctreeitem.cpp \
    epctreemodel.cpp \
    epcdb.cpp

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc

win32 {
    RC_FILE = tiretag.rc
}












