#-------------------------------------------------
#
# Project created by QtCreator 2016-08-15T19:06:07
#
#-------------------------------------------------

QT       += core gui widgets network xml

CONFIG += c++11

TARGET = TVControlPoint
TEMPLATE = app


SOURCES += \
    main.cpp \
    tvcontrolpoint.cpp

HEADERS  += \
    tvcontrolpoint.h

CONFIG += mobility
MOBILITY =

FORMS += \
    tvcontrolpoint.ui


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../upnp/release/ -lBrisaUpnp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../upnp/debug/ -lBrisaUpnp
else:unix: LIBS += -L$$OUT_PWD/../../upnp/ -lBrisaUpnp

INCLUDEPATH += $$PWD/../../upnp
INCLUDEPATH += $$PWD/../../upnp/controlpoint
INCLUDEPATH += $$PWD/../../upnp/device
INCLUDEPATH += $$PWD/../../upnp/herqq_http
INCLUDEPATH += $$PWD/../../upnp/ssdp
DEPENDPATH += $$PWD/../../upnp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../qtsoap/release/ -lqtsoap
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../qtsoap/debug/ -lqtsoap
else:unix: LIBS += -L$$OUT_PWD/../../qtsoap/ -lqtsoap

INCLUDEPATH += $$PWD/../../qtsoap
DEPENDPATH += $$PWD/../../qtsoap

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../core/release/ -lBrisaCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../core/debug/ -lBrisaCore
else:unix: LIBS += -L$$OUT_PWD/../../core/ -lBrisaCore

INCLUDEPATH += $$PWD/../../core
DEPENDPATH += $$PWD/../../core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../utils/release/ -lBrisaUtils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../utils/debug/ -lBrisaUtils
else:unix: LIBS += -L$$OUT_PWD/../../utils/ -lBrisaUtils

INCLUDEPATH += $$PWD/../../utils
DEPENDPATH += $$PWD/../../utils

RESOURCES += \
    figs.qrc
