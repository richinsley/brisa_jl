#-------------------------------------------------
#
# Project created by QtCreator 2016-08-14T11:15:04
#
#-------------------------------------------------

QT       += network xml

QT       -= gui

TARGET = BrisaUtils
TEMPLATE = lib

DEFINES += BUILD_BRISA_UTILS

SOURCES += \
    brisalog.cpp \
    brisanetwork.cpp

HEADERS += \
    brisalog.h \
    brisanetwork.h \
    brisautils.h

DEPENDPATH     += .
INCLUDEPATH    += . ../core

unix {
    target.path = /usr/lib
    INSTALLS += target
}
