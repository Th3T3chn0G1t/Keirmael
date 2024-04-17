// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

void kml_dputs(const char* s) {
	for(const char* c = s; *c; ++c) kml_dputc(*c);
}

void kml_dputx(kml_u64_t v) {
	static const char table[] = "0123456789ABCDEF";

	char buf[17] = { 0 };

	for(kml_size_t i = 16; i > 0; --i) {
		buf[i - 1] = table[v % 16];
		v /= 16;
	}

	kml_dputs(buf);
}
