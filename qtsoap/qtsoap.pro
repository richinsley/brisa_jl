#-------------------------------------------------
#
# Project created by QtCreator 2016-08-14T10:25:32
#
#-------------------------------------------------

QT       += network xml

QT       -= gui

TARGET = qtsoap
TEMPLATE = lib

DEFINES += QTSOAP_LIBRARY

SOURCES += \
    qtsoap.cpp

HEADERS +=\
        qtsoap_global.h \
    qtsoap.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
