# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by top level makefile.

KERNEL = keirmael$(SEP)kernel$(SEP)

KERNEL_SRC = $(KERNEL)start.c
KERNEL_OBJ = $(KERNEL_SRC:.c=$(KOBJ))

KERNEL_OUT = keirmael$(SEP)boot$(SEP)kernel$(KOUT)

$(KERNEL_OUT): $(KERNEL_OBJ)
