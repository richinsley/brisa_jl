#-------------------------------------------------
#
# Project created by QtCreator 2016-08-14T11:29:32
#
#-------------------------------------------------

QT       += core network xml gui

TARGET = BrisaUpnp
TEMPLATE = lib

DEFINES += BUILD_BRISA_UPNP

INCLUDEPATH +=     ./ssdp \
    ./controlpoint \
    ./device \
    ./herqq_http

SOURCES += \
    brisaabstracteventsubscription.cpp \
    brisaabstractservice.cpp \
    brisaaction.cpp \
    brisaargument.cpp \
    brisaicon.cpp \
    brisaservicexmlhandler.cpp \
    brisastatevariable.cpp \
    ssdp/brisassdpclient.cpp \
    ssdp/brisassdpserver.cpp \
    device/brisaabstracteventmessage.cpp \
    device/brisaactionxmlparser.cpp \
    device/brisacontrolwebservice.cpp \
    device/brisadevice.cpp \
    device/brisadevicexmlhandler.cpp \
    device/brisaeventcontroller.cpp \
    device/brisaeventmessage.cpp \
    device/brisaeventsubscription.cpp \
    device/brisamulticasteventmessage.cpp \
    device/brisaservice.cpp \
    controlpoint/brisacontrolpoint.cpp \
    controlpoint/brisacontrolpointdevice.cpp \
    controlpoint/brisacontrolpointservice.cpp \
    controlpoint/brisadevicexmlhandlercp.cpp \
    controlpoint/brisaeventproxy.cpp \
    controlpoint/brisamsearchclientcp.cpp \
    controlpoint/brisamulticasteventreceiver.cpp \
    controlpoint/brisaudplistener.cpp \
    herqq_http/hhttp_header_p.cpp

HEADERS += \
    brisaabstracteventsubscription.h \
    brisaabstractservice.h \
    brisaaction.h \
    brisaargument.h \
    brisaicon.h \
    brisaservicexmlhandler.h \
    brisastatevariable.h \
    ssdp/brisassdpclient.h \
    ssdp/brisassdpserver.h \
    device/brisaabstracteventmessage.h \
    device/brisaactionxmlparser.h \
    device/brisacontrolwebservice.h \
    device/brisadevice.h \
    device/brisadevicexmlhandler.h \
    device/brisaeventcontroller.h \
    device/brisaeventmessage.h \
    device/brisaeventsubscription.h \
    device/brisamulticasteventmessage.h \
    device/brisaservice.h \
    controlpoint/brisacontrolpoint.h \
    controlpoint/brisacontrolpointdevice.h \
    controlpoint/brisacontrolpointservice.h \
    controlpoint/brisadevicexmlhandlercp.h \
    controlpoint/brisaeventproxy.h \
    controlpoint/brisamsearchclientcp.h \
    controlpoint/brisamulticasteventreceiver.h \
    controlpoint/brisaudplistener.h \
    herqq_http/hhttp_header_p.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    LIBS += -lWs2_32
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qtsoap/release/ -lqtsoap
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qtsoap/debug/ -lqtsoap
else:unix: LIBS += -L$$OUT_PWD/../qtsoap/ -lqtsoap

INCLUDEPATH += $$PWD/../qtsoap
DEPENDPATH += $$PWD/../qtsoap

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lBrisaCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lBrisaCore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lBrisaCore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../utils/release/ -lBrisaUtils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../utils/debug/ -lBrisaUtils
else:unix: LIBS += -L$$OUT_PWD/../utils/ -lBrisaUtils

INCLUDEPATH += $$PWD/../utils
DEPENDPATH += $$PWD/../utils
