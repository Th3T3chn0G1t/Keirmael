// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

static void kml_dputs(const char* s) {
	for(const char* c = s; *c; ++c) kml_dputc(*c);
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
				kml_u64_t v = KML_VA_ARG(ap, kml_u64_t);

				for(kml_size_t i = 16; i > 0; --i) {
					kml_u64_t m = (v >> (4 * (i - 1))) & 0xF;
					kml_dputc("0123456789ABCDEF"[m]);
				}

				break;
			}

			case 'D': {
				kml_u64_t v = KML_VA_ARG(ap, kml_u64_t);

				char buf[20] = { 0 };
				kml_size_t i = sizeof(buf) - 1;
				do {
					buf[i--] = '0' + (v % 10);
				} while((v /= 10));
				i++;

				for(; i < sizeof(buf); ++i) kml_dputc(buf[i]);
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

	KML_DLOG("$S: $S", proc, resnames[result]);
}
