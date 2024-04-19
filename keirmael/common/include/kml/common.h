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
typedef kml_u64_t kml_size_t;

typedef enum { KML_FALSE, KML_TRUE } kml_bool_t;

enum kml_result {
	KML_OK,
	KML_E_UNKNOWN,
	KML_E_OOM,
	KML_E_RANGE,
	KML_E_TYPE,
	KML_E_PARAM,
	KML_E_ALIGN,
	KML_E_LATE
};

#define KML_ROUNDDOWN(x, fac) ((x) / (fac) * (fac))
#define KML_ROUNDUP(x, fac) (KML_ROUNDDOWN((x) + (fac) - 1, fac))

void* kml_memset(void*, int, kml_size_t);
void* kml_memcpy(void*, const void*, kml_size_t);

// TODO: This isn't particularly portable but `__has__builtin` doesn't seem to
//		 Work correctly.
#ifdef __GNUC__
typedef __builtin_va_list kml_va_list_t;
# define KML_VA_START(ap) __builtin_va_start((ap), 0)
# define KML_VA_END(ap) __builtin_va_end((ap))
# define KML_VA_ARG(ap, t) __builtin_va_arg((ap), t)
#endif

#endif
