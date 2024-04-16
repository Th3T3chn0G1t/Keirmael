// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

#include <kmlk/kernel.h>

// Arch specific init. calls us here.
void kmlk_start(void) {
	kml_dputs("Hello, Keirmael!\n");

	kmlk_hang();
}
