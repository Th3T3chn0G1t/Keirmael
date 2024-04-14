# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by top level makefile.

ifeq ($(IMAGE_MODE),iso)
IMAGE_OUT = keirmael.iso
IMAGE_MBR = boot$(SEP)hyper$(SEP)mbr.bin

# xorriso doesn't behave correctly with `\\`.
MKISO_FLAGS = -b $(subst $(SEP),/,$(IMAGE_MBR)) --mbr-force-bootable --protective-msdos-label
MKISO_FLAGS += -no-emul-boot -boot-load-size 4 -boot-info-table

all: $(IMAGE_OUT)
$(IMAGE_OUT): $(KERNEL_OUT) $(BOOT_OUT) $(BOOT_INSTALL)

$(IMAGE_OUT):
	$(CP) $(BOOT_OUT) keirmael$(SEP)boot$(SEP)$(IMAGE_MBR)
	$(XORRISO) -as mkisofs $(MKISO_FLAGS) keirmael/boot -o keirmael.iso
	$(BOOT_INSTALL) $@ iso
	$(RM) keirmael$(SEP)boot$(SEP)$(IMAGE_MBR)

clean: clean_image
.PHONY: clean_image
clean_image:
	$(RM) $(IMAGE_OUT)
endif
