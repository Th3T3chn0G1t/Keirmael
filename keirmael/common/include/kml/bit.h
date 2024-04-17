// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KML_BIT_H
#define KML_BIT_H

#include <kml/common.h>

void kml_bitset_set(kml_u8_t*, kml_size_t);
void kml_bitset_unset(kml_u8_t*, kml_size_t);
kml_bool_t kml_bitset_get(const kml_u8_t*, kml_size_t);

#endif
