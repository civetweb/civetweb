# 
# Copyright (c) 2013 No Face Press, LLC
# License http://opensource.org/licenses/mit-license.php MIT License
#

#This makefile is used to test the other Makefiles

TOP = ..
TEST_OUT = test_install

include $(TOP)/resources/Makefile.in-os

all: test

test: buildoptions buildlibs buildinstall
test: buildexamples threaded

ifeq ($(TARGET_OS),OSX)
test: dmg
endif

test: clean
	@echo PASSED

dmg:
	$(MAKE) -C $(TOP) -f Makefile.osx clean package

buildexamples:
	$(MAKE) -C $(TOP)/examples/chat clean all
	$(MAKE) -C $(TOP)/examples/chat clean
	$(MAKE) -C $(TOP)/examples/hello clean all
	$(MAKE) -C $(TOP)/examples/hello clean
	$(MAKE) -C $(TOP)/examples/post clean all
	$(MAKE) -C $(TOP)/examples/post clean
	$(MAKE) -C $(TOP)/examples/upload clean all
	$(MAKE) -C $(TOP)/examples/upload clean
	$(MAKE) -C $(TOP)/examples/websocket clean all
	$(MAKE) -C $(TOP)/examples/websocket clean
	$(MAKE) -C $(TOP)/examples/embedded_cpp clean all
	$(MAKE) -C $(TOP)/examples/embedded_cpp clean

buildoptions:
	$(MAKE) -C $(TOP) clean build
	$(MAKE) -C $(TOP) clean build WITH_IPV6=1
	$(MAKE) -C $(TOP) clean build WITH_WEBSOCKET=1
	$(MAKE) -C $(TOP) clean build WITH_LUA=1
	$(MAKE) -C $(TOP) clean build WITH_LUA=1 WITH_IPV6=1 WITH_WEBSOCKET=1

threaded:
	$(MAKE) -j 8 -C $(TOP) clean build WITH_LUA=1

buildinstall:
	$(MAKE) -C $(TOP) clean install PREFIX=$(TEST_OUT)

buildlibs:
	$(MAKE) -C $(TOP) clean lib
	$(MAKE) -C $(TOP) clean slib
	$(MAKE) -C $(TOP) clean lib WITH_CPP=1
	$(MAKE) -C $(TOP) clean slib WITH_CPP=1

clean:
	$(MAKE) -C $(TOP) clean
	rm -rf $(TOP)/$(TEST_OUT)

.PHONY: all buildoptions buildinstall clean os linux
