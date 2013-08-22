# 
# Copyright (c) 2013 No Face Press, LLC
# License http://opensource.org/licenses/mit-license.php MIT License
#

#
# For help try, "make help"
#

include build/Makefile.in-os

CPROG = civetweb
#CXXPROG = civetweb

BUILD_DIR = out
INSTALL_DIR = /

BUILD_DIRS += $(BUILD_DIR)

LIB_SOURCES = civetweb.c md5.c 
APP_SOURCES = main.c
SOURCE_DIRS =

OBJECTS = $(LIB_SOURCES:.c=.o) $(APP_SOURCES:.c=.o)

# only set main compile options if none were chosen
CFLAGS += -W -Wall -O2 -D$(TARGET_OS) $(COPT)

ifdef WITH_DEBUG
  CFLAGS += -g -DDEBUG_ENABLED
endif

ifdef WITH_LUA
 include build/Makefile.in-lua
endif

ifdef WITH_IPV6
  CFLAGS += -DDSE_IPV6
endif

ifdef WITH_WEBSOCKET
  CFLAGS += -DUSE_WEBSOCKET
endif

ifdef CONFIG_FILE
  CFLAGS += -DCONFIG_FILE=\"$(CONFIG_FILE)\"
endif

ifdef CONFIG_FILE2
  CFLAGS += -DCONFIG_FILE2=\"$(CONFIG_FILE2)\"
endif

ifdef SSL_LIB
  CFLAGS += -DSSL_LIB=\"$(SSL_LIB)\"
endif

ifdef CRYPTO_LIB
  CFLAGS += -DCRYPTO_LIB=\"$(CRYPTO_LIB)\"
endif

BUILD_DIRS += $(addprefix $(BUILD_DIR)/, $(SOURCE_DIRS))
BUILD_OBJECTS = $(addprefix $(BUILD_DIR)/, $(OBJECTS))

LIBS = -lpthread -lm

ifeq ($(TARGET_OS),LINUX) 
	LIBS += -ldl
endif

all: build

help:
	@echo "make help                show this message"
	@echo "make build               compile"
	@echo "make install             install on the system"
	@echo "make clean               clean up the mess"
	@echo ""
	@echo " Make Options"
	@echo "   WITH_LUA=1            build with LUA support"
	@echo "   WITH_DEBUG=1          build with GDB debug support"
	@echo "   WITH_IPV6=1           with IPV6 support"
	@echo "   WITH_WEBSOCKET=1      build with web socket support"
	@echo "   CONFIG_FILE=file      use 'file' as the config file"
	@echo "   CONFIG_FILE2=file     use 'file' as the backup config file"
	@echo "   SSL_LIB=libssl.so.0   use versioned SSL library"
	@echo "   CRYPTO_LIB=libcrypto.so.0 system versioned CRYPTO library"
	@echo "   INSTALL_DIR=/         sets the install directory"
	@echo "   COPT='-DNO_SSL'       method to insert compile flags"
	@echo ""
	@echo " Compile Flags"
	@echo "   NDEBUG                strip off all debug code"
	@echo "   DEBUG                 build debug version (very noisy)"
	@echo "   NO_CGI                disable CGI support"
	@echo "   NO_SSL                disable SSL functionality"
	@echo "   NO_SSL_DL             link against system libssl library"
	@echo ""
	@echo " Variables"
	@echo "   TARGET_OS='$(TARGET_OS)'"
	@echo "   CFLAGS='$(CFLAGS)'"
	@echo "   CXXFLAGS='$(CXXFLAGS)'"
	@echo "   LDFLAGS='$(LDFLAGS)'"
	@echo "   CC='$(CC)'"
	@echo "   CXX='$(CXX)'"

build: $(CPROG) $(CXXPROG)

install: build
ifeq ($(TARGET_OS),LINUX)
	install -Dm755 "$(CPROG)" "$(INSTALL_DIR)/usr/bin/$(CPROG)"
	install -Dm644 "distribution/arch/$(CPROG).conf" "$(INSTALL_DIR)/etc/$(CPROG)/$(CPROG).conf"
	install -d "$(INSTALL_DIR)/usr/share/$(CPROG)"
	install -m644 "UserManual.md" "README.md" "$(INSTALL_DIR)/usr/share/$(CPROG)"
endif


clean:
	rm -rf $(BUILD_DIR)

$(CPROG): $(BUILD_DIRS) $(BUILD_OBJECTS)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(BUILD_OBJECTS) $(LIBS)

$(CXXPROG): $(BUILD_DIRS) $(BUILD_OBJECTS)
	$(CXX) -o $@ $(CFLAGS) $(LDFLAGS) $(BUILD_OBJECTS) $(LIBS)

$(BUILD_DIRS):
	-@mkdir -p $@

$(BUILD_DIR)/%.o : %.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: all help build install clean
