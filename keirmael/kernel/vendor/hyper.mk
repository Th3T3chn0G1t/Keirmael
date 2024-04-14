# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by arch tools makefile.

HYPER = $(KVENDOR)Hyper$(SEP)

HYPER_CACHE = $(HYPER)build

BOOT_KCFLAGS = -isystem $(HYPER)loader$(SEP)boot_protocol$(SEP)ultra_protocol
BOOT_OUT = $(HYPER_CACHE)$(SEP)loader$(SEP)hyper_iso_boot

BOOT_INSTALL = $(HYPER)installer$(SEP)hyper_install
ifdef WINDOWS
BOOT_INSTALL := $(BOOT_INSTALL).exe
endif

HYPER_MAKEFILE = $(HYPER_CACHE)$(SEP)Makefile

HYPER_CMAKEFLAGS = -DCMAKE_C_COMPILER="$(KCC)" -DCMAKE_LINKER="$(KLD)"
HYPER_CMAKEFLAGS += -DCMAKE_ASM_NASM_COMPILER="$(NASM)"
HYPER_CMAKEFLAGS += -DCMAKE_C_FLAGS="$(CFLAGS)"
HYPER_CMAKEFLAGS += -DHYPER_PLATFORM=bios -DHYPER_ARCH=i686
HYPER_CMAKEFLAGS += -DHYPER_TOOLCHAIN=..$(SEP)hyper-toolchain.cmake

HYPER_CMAKEFLAGS += -G "Unix Makefiles" -DCMAKE_MAKE_PROGRAM="$(MAKE)"

ifdef DEBUG
	HYPER_CMAKEFLAGS += -DCMAKE_BUILD_TYPE=Debug
else
	HYPER_CMAKEFLAGS += -DCMAKE_BUILD_TYPE=Release
endif

$(HYPER_CACHE):
	-mkdir $@

$(HYPER_MAKEFILE): $(HYPER)CMakeLists.txt | $(HYPER_CACHE)
	$(CMAKE) -B$(HYPER_CACHE) $(dir $<) $(HYPER_CMAKEFLAGS)

$(BOOT_OUT) $(BOOT_INSTALL): $(HYPER_MAKEFILE)
	$(MAKE) -C $(dir $<)

clean: clean_hyper
.PHONY: clean_hyper
clean_hyper:
	-$(RMDIR) $(HYPER_CACHE)
