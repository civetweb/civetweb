# 
# Copyright (c) 2013 No Face Press, LLC
# License http://opensource.org/licenses/mit-license.php MIT License
#
################################################################################
#
# civetweb
#
################################################################################

CIVETWEB_VERSION = 1.2
CIVETWEB_SOURCE = civetweb-$(CIVETWEB_VERSION).tar.gz
CIVETWEB_SITE = http://github.com/sunsetbrew/civetweb/tarball/v$(CIVETWEB_VERSION)
CIVETWEB_LICENSE = MIT
CIVETWEB_LICENSE_FILES = LICENSE.md

CIVETWEB_COPT = $(TARGET_CFLAGS)
CIVETWEB_MOPT = TARGET_OS=LINUX
CIVETWEB_LDFLAGS = $(TARGET_LDFLAGS)

ifndef BR2_PACKAGE_UTIL_LINUX_FALLOCATE
	CIVETWEB_COPT += -DHAVE_POSIX_FALLOCATE=0
endif

ifdef BR2_CIVETWEB_WITH_IPV6
	CIVETWEB_MOPT += WITH_IPV6=1
endif

ifdef BR2_CIVETWEB_WITH_LUA
	CIVETWEB_MOPT += WITH_LUA=1
endif

ifdef BR2_CIVETWEB_WITH_SSL
	CIVETWEB_COPT += -DNO_SSL_DL -lcrypt -lssl
	CIVETWEB_DEPENDENCIES += openssl
else
	CIVETWEB_COPT += -DNO_SSL
endif

define CIVETWEB_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(@D) all $(CIVETWEB_MOPT) COPT="$(CIVETWEB_COPT)"
endef

define CIVETWEB_INSTALL_TARGET_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(@D) install DOCUMENT_ROOT=/usr/local/share/doc/civetweb PREFIX="$(TARGET_DIR)/usr/local" $(CIVETWEB_MOPT) COPT='$(CIVETWEB_COPT)'
endef

$(eval $(generic-package))