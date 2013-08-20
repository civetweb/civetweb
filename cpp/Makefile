# 
# Copyright (c) 2013 No Face Press, LLC
# License http://opensource.org/licenses/mit-license.php MIT License
#

CFLAGS=	-W -Wall -Wno-unused-parameter -I.. -I. -g
LIB_SOURCES = CivetServer.cpp ../civetweb.c ../build/md5.c
LIBS = -lpthread 

all:
	$(CXX) $(CFLAGS) example.cpp $(LIB_SOURCES) $(LIBS) -o example;

MSVC    = e:/vc6
CL      = $(MSVC)/bin/cl
CLFLAGS = /MD /TC /nologo $(DBG) /W3 /DNO_SSL \
        /I$(MSVC)/include /I.. /I. /Dsnprintf=_snprintf \
        /link /incremental:no /libpath:$(MSVC)/lib /machine:IX86

windows:
	$(CL) example.cpp $(LIB_SOURCES) $(CLFLAGS)

clean:
	rm -rf example *.exe *.dSYM *.obj
