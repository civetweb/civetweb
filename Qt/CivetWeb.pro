TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../src/md5.inl \
    ../src/mod_lua.inl \
    ../src/timer.inl \
    ../src/civetweb.c \
    ../src/main.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    ../include/civetweb.h

INCLUDEPATH +=  \
    ../include/

LIBS += -lws2_32 -lComdlg32

DEFINES += USE_IPV6
DEFINES += USE_WEBSOCKET
