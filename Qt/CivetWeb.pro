TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../src/md5.inl \
    ../src/sha1.inl \
    ../src/handle_form.inl \
    ../src/mod_lua.inl \
    ../src/mod_duktape.inl \
    ../src/timer.inl \
    ../src/civetweb.c \
    ../src/main.c

#include(deployment.pri)
#qtcAddDeployment()

HEADERS += \
    ../include/civetweb.h

INCLUDEPATH +=  \
    ../include/

win32 {
LIBS += -lws2_32 -lComdlg32 -lUser32 -lShell32 -lAdvapi32
} else {
LIBS += -lpthread -ldl -lm
}


DEFINES += USE_IPV6
DEFINES += USE_WEBSOCKET
DEFINES += USE_SERVER_STATS
