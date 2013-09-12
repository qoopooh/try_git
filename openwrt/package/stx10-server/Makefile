#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=stx10-server
PKG_RELEASE:=2
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/stx10-server
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=UHF RFID Server
endef

define Package/stx10-server/description
	TCP Server providing UHF RFID STx10 protocol
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS) -I$(LINUX_DIR)/include"
endef

define Package/stx10-server/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/stx10-server $(1)/usr/bin/
endef

$(eval $(call BuildPackage,stx10-server))
