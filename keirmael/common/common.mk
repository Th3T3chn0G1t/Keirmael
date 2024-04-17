# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by top level makefile.

COMMON = keirmael$(SEP)common$(SEP)

COMMON_HDR = $(wildcard keirmael/common/include/*.h)
COMMON_SRC = $(COMMON)io.c
COMMON_OBJ = $(COMMON_SRC:.c=$(KOBJ))

COMMON_OUT = $(COMMON)libcommon$(KA)

$(COMMON_SRC): $(COMMON_HDR)

$(COMMON_OUT): KCFLAGS += -I$(COMMON)include
$(COMMON_OUT): $(COMMON_OBJ)

clean: clean_common
.PHONY: clean_common
clean_common:
	-$(RM) $(COMMON_OBJ)
	-$(RM) $(COMMON_OUT)