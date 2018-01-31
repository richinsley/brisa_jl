QT += core gui widgets network xml

CONFIG += c++11

TARGET = BrisaMediaServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    brisamediaserver.cpp \
    brisamediaserverabstractplugin.cpp \
    connectionmanager.cpp \
    contentdirectory.cpp \
    main.cpp \
    mediaserver.cpp \
    pluginmanager.cpp \
    DIDL/audioitem.cpp \
    DIDL/container.cpp \
    DIDL/createclass.cpp \
    DIDL/didlobject.cpp \
    DIDL/genre.cpp \
    DIDL/item.cpp \
    DIDL/resource.cpp \
    DIDL/searchclass.cpp \
    DIDL/videoitem.cpp

HEADERS += \
    brisamediaserver.h \
    brisamediaserverabstractplugin.h \
    connectionmanager.h \
    contentdirectory.h \
    mediaserver.h \
    pluginmanager.h \
    DIDL/audioitem.h \
    DIDL/container.h \
    DIDL/createclass.h \
    DIDL/didlobject.h \
    DIDL/genre.h \
    DIDL/item.h \
    DIDL/resource.h \
    DIDL/searchclass.h \
    DIDL/videoitem.h

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
    brisamediaserver.qrc
