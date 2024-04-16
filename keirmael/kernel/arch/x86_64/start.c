// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kmlk/kernel.h>

#include <kmlk/arch/x86_64/tables.h>
#include <kmlk/arch/x86_64/ports.h>

/* This is just here temporarily while we work out full logging infra. */
static void puts(const char* s) {
	for(const char* c = s; *c; ++c) kmlk_outb(KMLK_PORT_DEBUG, *c);
}

void _start(void) {
	asm("cli");

	// Random arch guff (gdt, idt+pic, tss etc.)
	kmlk_set_arch_tables();

	// Mem. map

	puts("Hello, Keirmael!\n");

	kmlk_start();
}
