# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Set up toolchain vars/targets.
include keirmael/defaults.mk
include keirmael/host/tools.mk
# TODO: Document what variables/targets dependent Makefiles are expected to
#		Export for porting etc.
include keirmael/kernel/arch/$(ARCH)/tools.mk
include keirmael/user/tools.mk

# Include project targets.
include keirmael/common/common.mk
include keirmael/kernel/kernel.mk
include keirmael/image.mk

.DEFAULT_GOAL := all
.PHONY: all clean
