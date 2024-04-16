# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by kernel makefile.

# TODO: Make this a separate lib target.
ARCH_SRC = $(KARCH)start.c $(KARCH)kio.c $(KARCH)tables.c $(KARCH)vectors.c
ARCH_SRC += $(KARCH)interrupt.c
