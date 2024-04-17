// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

#include <kmlk/arch/x86_64/ports.h>

void kml_dputc(char c) {
	kmlk_outb(KMLK_PORT_DEBUG, c);
}
