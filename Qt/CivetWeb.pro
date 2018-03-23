TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += MG_EXPERIMENTAL_INTERFACES

SOURCES += \
    ../src/md5.inl \
    ../src/sha1.inl \
    ../src/handle_form.inl \
    ../src/mod_lua.inl \
    ../src/mod_duktape.inl \
    ../src/timer.inl \
    ../src/civetweb.c \
    ../src/main.c \
    ../src/mod_zlib.inl

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

#To build with DEBUG traces:
#
#DEFINES += DEBUG

linux {
INCLUDEPATH +=  \
    ../src/third_party/ \
    ../src/third_party/lua-5.2.4/src

DEFINES += USE_LUA
DEFINES += USE_LUA_SHARED
LIBS += -llua5.2

DEFINES += USE_ZLIB
LIBS += -lz
}

#To build with duktape support:
#
#INCLUDEPATH +=  \
#    ../src/third_party/duktape-1.8.0/src
#
#DEFINES += USE_DUKTAPE
