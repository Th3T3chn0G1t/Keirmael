// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/bit.h>

void kml_bitset_set(kml_u8_t* bitset, kml_size_t n) {
	bitset[n / 8] |= 1 << (n % 8);
}

void kml_bitset_unset(kml_u8_t* bitset, kml_size_t n) {
	bitset[n / 8] &= ~(1 << (n % 8));
}

kml_bool_t kml_bitset_get(const kml_u8_t* bitset, kml_size_t n) {
	return !!(bitset[n / 8] & (1 << (n % 8)));
}
