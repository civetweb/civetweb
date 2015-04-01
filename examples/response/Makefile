# 
# Copyright (c) 2015 the Civetweb developers
# License http://opensource.org/licenses/mit-license.php MIT License
#

#Build the response example demonstrating use of response helper functions


PROG = response
SRC = response.c

TOP = ../..
CIVETWEB_LIB = libcivetweb.a

CFLAGS = -I$(TOP)/include $(COPT)
LIBS = -lpthread

include $(TOP)/resources/Makefile.in-os

ifeq ($(TARGET_OS),LINUX) 
	LIBS += -ldl
endif

all: $(PROG)

$(PROG): $(CIVETWEB_LIB) $(SRC)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(SRC) $(CIVETWEB_LIB) $(LIBS)

$(CIVETWEB_LIB):
	$(MAKE) -C $(TOP) clean lib
	cp $(TOP)/$(CIVETWEB_LIB) .

clean:
	rm -f $(CIVETWEB_LIB) $(PROG)

.PHONY: all clean
