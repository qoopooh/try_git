#-------------------------------------------------
#
# Project created by QtCreator 2014-01-06T16:41:08
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network

TARGET = A-cmd
TEMPLATE = app

SOURCES += main.cpp \
    chat.cpp \
    gatewaymessage.cpp \
    encrypt.cpp \
    register.cpp

HEADERS += \
    chat.h \
    gatewaymessage.h \
    encrypt.h \
    register.h \
    def.h

macx {
    ICON = icon-mac.icns
}
#  RC_FILE = win.rc


ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

RESOURCES += \
    resource.qrc
