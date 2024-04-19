# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by top level makefile.

UCC = clang
ULD = ld.lld
UAR = llvm-ar
URANLIB = llvm-ranlib

UOBJ = .o.kml
UA = .a.kml
UOUT = .elf.kml

# TODO: This just inherits kernel cflags while we haven't set up a more sane
#		Userland environment.
SET_UCFLAGS = $(SET_KCFLAGS)

%$(UOBJ): UCFLAGS += $(SET_UCFLAGS)
%$(UOBJ): CFLAGS += $(SET_CFLAGS)
%$(UOBJ): %.c
	$(UCC) -c -o $@ $< $(CFLAGS) $(UCFLAGS)

%$(UA):
	$(UAR) rc $@ $(filter %$(UOBJ),$^)
	$(URANLIB) $@

%$(UOUT):
	$(ULD) -o $@ $(LDFLAGS) $(ULDFLAGS) $(filter %$(KOBJ),$^) $(LDLIBS) $(ULDLIBS)
