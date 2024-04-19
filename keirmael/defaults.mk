# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by top level makefile.

CMAKE = cmake
XORRISO = xorriso
NASM = nasm

IMAGE_MODE = iso
ARCH = x86_64

SET_CFLAGS = -std=gnu23

ifdef DEBUG
	SET_CFLAGS += -g -D_DEBUG
else
	SET_CFLAGS += -O3 -DNDEBUG
endif

ifdef MAINTAINER
	SET_CFLAGS += -pedantic -Wall -Wextra -Werror
	# For `asm`.
	SET_CFLAGS += -Wno-language-extension-token
	# This is incorrectly issued under `-pedantic` in C23 mode.
	# https://github.com/llvm/llvm-project/issues/89396.
	SET_CFLAGS += -Wno-gnu-binary-literal
endif
