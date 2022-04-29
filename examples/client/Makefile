#
# Copyright (c) 2021 CivetWeb Developers
# License http://opensource.org/licenses/mit-license.php MIT License

TOP = ../..

PROG = client
SRC = client.c $(TOP)/src/civetweb.c

CFLAGS = -I$(TOP)/include -DOPENSSL_API_1_1 -DCRYPTO_LIB=\"libcrypto.so.1.1\" -DSSL_LIB=\"libssl.so.1.1\" $(COPT)
LIBS = -lpthread

include $(TOP)/resources/Makefile.in-os

ifeq ($(TARGET_OS),LINUX)
	LIBS += -ldl
endif

all: $(PROG)

$(PROG): $(CIVETWEB_LIB) $(SRC)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(SRC) $(CIVETWEB_LIB) $(LIBS)

$(CIVETWEB_LIB):
	$(MAKE) -C $(TOP) WITH_IPV6=1 clean lib
	cp $(TOP)/$(CIVETWEB_LIB) .

clean:
	rm -f $(CIVETWEB_LIB) $(PROG)

.PHONY: all clean

