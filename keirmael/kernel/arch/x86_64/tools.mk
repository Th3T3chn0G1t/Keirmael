# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by top level makefile.

KCC = clang
KLD = ld.lld
NASM = nasm

include keirmael/kernel/vendor/hyper.mk

KOBJ = .o.$(ARCH)
KOUT = .elf

SET_KCFLAGS = --target=x86_64-unknown-elf -ffreestanding -static
SET_KCFLAGS += -fno-pic -mcmodel=kernel -fno-builtin -mno-red-zone
SET_KCFLAGS += -mno-stack-arg-probe -fno-strict-aliasing -mgeneral-regs-only
SET_KCFLAGS += -fno-threadsafe-statics

%$(KOBJ): KCFLAGS += $(SET_KCFLAGS)
%$(KOBJ): CFLAGS += $(SET_CFLAGS)
%$(KOBJ): %.c
	$(KCC) -c -o $@ $< $(CFLAGS) $(KCFLAGS)

%$(KOUT):
	$(KLD) -o $@ $(LDFLAGS) $(KLDFLAGS) $(filter %$(KOBJ),$^) $(LDLIBS)
