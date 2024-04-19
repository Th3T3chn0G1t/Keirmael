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

void kml_dputf(const char* fmt, ...) {
	kml_va_list_t ap;

	KML_VA_START(ap);

	for(; *fmt; ++fmt) {
		if(*fmt != '$') {
			kml_dputc(*fmt);
			continue;
		}

		switch(*++fmt) {
			default: {
				kml_dputc('$');
				kml_dputc(*fmt);
				break;
			}

			case '$': {
				kml_dputc('$');
				break;
			}

			case 'S': {
				kml_dputs(KML_VA_ARG(ap, const char*));
				break;
			}

			case 'X': {
				kml_dputx(KML_VA_ARG(ap, kml_u64_t));
				break;
			}
		}
	}

	KML_VA_END(ap);
}

void kml_presult(const char* proc, enum kml_result result) {
	static const char* resnames[] = {
			[KML_OK] = "no error",
			[KML_E_UNKNOWN] = "unknown error",
			[KML_E_OOM] = "out of memory",
			[KML_E_RANGE] = "out of range",
			[KML_E_TYPE] = "wrong type",
			[KML_E_PARAM] = "bad parameter",
			[KML_E_ALIGN] = "bad alignment",
			[KML_E_LATE] = "too late"
	};

	kml_dputs(proc);
	kml_dputs(": ");
	kml_dputs(resnames[result]);
	kml_dputc('\n');
}
