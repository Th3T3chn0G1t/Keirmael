# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by top level makefile.

CMAKE = cmake

ARCH = x86_64

SET_CFLAGS = -std=gnu23

ifdef DEBUG
	SET_CFLAGS += -g -D_DEBUG
else
	SET_CFLAGS += -O3 -DNDEBUG
endif

ifdef MAINTAINER
	SET_CFLAGS += -pedantic -pedantic-errors -Wall -Wextra -Werror
endif
