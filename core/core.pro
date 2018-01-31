#-------------------------------------------------
#
# Project created by QtCreator 2016-08-14T10:36:26
#
#-------------------------------------------------

QT       += core network xml
include(../brisa_jl_config.pri)

QT       -= gui

TARGET = BrisaCore
TEMPLATE = lib

DEFINES += BUILD_BRISA_CORE

SOURCES += \
    httpsessionmanager.cpp \
    httpsession.cpp \
    httpserver.cpp \
    httpresponse.cpp \
    httprequest.cpp \
    httpmessage.cpp \
    brisawebstaticcontent.cpp \
    brisawebservice.cpp \
    brisawebserversession.cpp \
    brisawebserver.cpp \
    brisawebfile.cpp \
    brisaconfig.cpp

HEADERS +=\
        core_global.h \
    httpversion.h \
    httpsessionmanager.h \
    httpsession.h \
    httpserver.h \
    httpresponse.h \
    httprequest.h \
    httpmessage.h \
    brisawebstaticcontent.h \
    brisawebservice.h \
    brisawebserversession.h \
    brisawebserver.h \
    brisawebfile.h \
    brisaglobal.h \
    brisacore.h \
    brisaconfig.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
