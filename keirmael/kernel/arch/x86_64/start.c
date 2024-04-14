// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kmlk/kernel.h>

static inline void outb(short port, unsigned char value) {
	__asm__("outb %[value], %[port]" :: [value]"a"(value), [port]"Nd"(port) :);
}

void _start(void) {
	static const char msg[] = "Hello, Keirmael!\n";

	for(const char* c = msg; *c; ++c) outb(0xE9, *c);

	kmlk_start();
}
