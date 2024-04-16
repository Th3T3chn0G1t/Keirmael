// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KML_COMMON_H
#define KML_COMMON_H

typedef unsigned char kml_u8_t;
typedef unsigned short kml_u16_t;
typedef unsigned int kml_u32_t;
typedef unsigned long kml_u64_t;

typedef char kml_i8_t;
typedef short kml_i16_t;
typedef int kml_i32_t;
typedef long kml_i64_t;

typedef kml_u64_t kml_ptr_t;

typedef enum { KML_FALSE, KML_TRUE } kml_bool_t;

enum kml_result {
	KML_OK,
	KML_ERROR
};

#define KML_CAT_IMPL(a, b) a ## b
#define KML_CAT(a, b) KML_CAT_IMPL(a, b)

#endif
