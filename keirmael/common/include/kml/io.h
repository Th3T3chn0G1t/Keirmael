// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KML_IO_H
#define KML_IO_H

#include <kml/common.h>

// Backend specific no-error "putchar".
void kml_dputc(char);

// Uses `$` for fmt -- very much not `printf` compatible.
void kml_dputf(const char*, ...);

#define KML_DLOG(fmt, ...) \
	kml_dputf( \
		"[$S:$D] " fmt "\n", \
		__FILE__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

void kml_presult(const char*, enum kml_result);

#endif
