# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by top level makefile.

KCC = clang
KLD = ld.lld
KAR = llvm-ar
KRANLIB = llvm-ranlib

BOOT_MAKE = keirmael$(SEP)kernel$(SEP)vendor$(SEP)hyper.mk

KOBJ = .o.$(ARCH)
KA = .a.$(ARCH)
KOUT = .elf.$(ARCH)

SET_KCFLAGS = --target=x86_64-unknown-elf -ffreestanding -static
SET_KCFLAGS += -fno-pic -mcmodel=kernel -fno-builtin -mno-red-zone
SET_KCFLAGS += -mno-stack-arg-probe -fno-strict-aliasing -mgeneral-regs-only
SET_KCFLAGS += -fno-threadsafe-statics

%$(KOBJ): KCFLAGS += $(SET_KCFLAGS)
%$(KOBJ): CFLAGS += $(SET_CFLAGS)
%$(KOBJ): %.c
	$(KCC) -c -o $@ $< $(CFLAGS) $(KCFLAGS)

%$(KA):
	$(KAR) rc $@ $(filter %$(KOBJ),$^)
	$(KRANLIB) $@

%$(KOUT):
	$(KLD) -o $@ $(LDFLAGS) $(KLDFLAGS) $(filter %$(KOBJ),$^) $(LDLIBS) $(KLDLIBS)
