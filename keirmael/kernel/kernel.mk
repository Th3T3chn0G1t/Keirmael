# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by top level makefile.

KERNEL = keirmael$(SEP)kernel$(SEP)
KVENDOR = $(KERNEL)vendor$(SEP)
KARCH = $(KERNEL)arch$(SEP)$(ARCH)$(SEP)

# TODO: Document what variables/targets dependent Makefiles are expected to
#		Export for porting etc.
include $(KARCH)tools.mk

include $(KARCH)arch.mk

# Define kernel target.

KERNEL_KCFLAGS = -I$(KERNEL)include -Ikeirmael$(SEP)common$(SEP)include

KERNEL_HDR = $(wildcard keirmael/kernel/include/*.h)
KERNEL_HDR += $(wildcard keirmael/kernel/include/arch/*.h)
KERNEL_HDR += $(wildcard keirmael/kernel/include/arch/$(ARCH)/*.h)
KERNEL_SRC = $(ARCH_SRC) $(KERNEL)start.c
KERNEL_OBJ = $(KERNEL_SRC:.c=$(KOBJ))

KERNEL_OUT = keirmael$(SEP)boot$(SEP)boot$(SEP)kernel$(KOUT)

$(KERNEL_SRC): $(KERNEL_HDR)

all: $(KERNEL_OUT)
$(KERNEL_OUT): KCFLAGS += $(KERNEL_KCFLAGS)
$(KERNEL_OUT): KLDFLAGS += -T$(KARCH)kernel.ld
$(KERNEL_OUT): $(KERNEL_OBJ)

# Add bootloader reqs..

$(KERNEL_OUT): KCFLAGS += $(BOOT_KCFLAGS)

clean: clean_kernel
.PHONY: clean_kernel
clean_kernel:
	-$(RM) $(KERNEL_OBJ)
	-$(RM) $(KERNEL_OUT)
