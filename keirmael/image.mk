# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by top level makefile.

IMAGE_OUT = keirmael$(SEP)keirmael.img

all: $(IMAGE_OUT)
$(IMAGE_OUT): $(BOOT_OUT)
