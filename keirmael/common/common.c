// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/common.h>

void* kml_memset(void* p, int c, kml_size_t n) {
	for(kml_size_t i = 0; i < n; ++i) ((char*) p)[i] = c;

	return p;
}

void* kml_memcpy(void* dest, const void* src, kml_size_t n) {
	char* dc = dest;
	const char* srcc = src;

	for(kml_size_t i = 0; i < n; ++i) dc[i] = srcc[i];

	return dest;
}
