// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KML_IO_H
#define KML_IO_H

#include <kml/common.h>

// Super simple no-error print string for debugging.
// TODO: We can remove this once we have better logging infrastructure in
//		 Place.
void kml_dputc(char);
void kml_dputs(const char*);
void kml_dputx(kml_u64_t);

// Uses `$` for fmt -- very much not `printf` compatible.
void kml_dputf(const char*, ...);

void kml_presult(const char*, enum kml_result);

#endif
