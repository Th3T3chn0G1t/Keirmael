// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KMLK_X86_64_PORTS_H
#define KMLK_X86_64_PORTS_H

#include <kml/common.h>

#define KMLK_PIC1 (0x20)
#define KMLK_PIC2 (0xA0)

#define KMLK_PORT_DEBUG (0xE9)

[[gnu::always_inline]] inline void kmlk_outb(kml_u16_t port, kml_u8_t value) {
	asm("outb %[value], %[port]" :: [value]"a"(value), [port]"Nd"(port) :);
}

[[gnu::always_inline]] inline void kmlk_outw(kml_u16_t port, kml_u16_t value) {
	asm("outw %[value], %[port]" :: [value]"a"(value), [port]"Nd"(port) :);
}

[[gnu::always_inline]] inline kml_u8_t alo_port_in_byte(kml_u16_t port) {
	kml_u8_t retval = 0;
	asm("inb %[port], %[retval]" : [retval]"=a"(retval) : [port]"Nd"(port) :);
	return retval;
}

#endif
