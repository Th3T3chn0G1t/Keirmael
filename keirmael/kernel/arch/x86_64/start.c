// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kmlk/kernel.h>

#include <kmlk/arch/x86_64/tables.h>

// TODO: Find a better place to put this.
[[noreturn]] void kmlk_hang(void) {
	asm("cli");
	while(1) asm("hlt");
}

void _start(void) {
	asm("cli");

	// Random arch guff (gdt, idt+pic, tss etc.)
	kmlk_set_arch_tables();

	// Mem. map

	kmlk_start();
}
