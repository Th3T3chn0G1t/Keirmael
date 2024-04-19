// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

#include <kmlk/kernel.h>
#include <kmlk/memory.h>

#include <kmlk/arch/x86_64/start.h>
#include <kmlk/arch/x86_64/tables.h>
#include <kmlk/arch/x86_64/interrupt.h>

// TODO: Find a better place to put this.
[[noreturn]] void kmlk_hang(void) {
	asm("cli");
	while(1) asm("hlt");
}

void _start(struct ultra_boot_context* ctx, unsigned int magic) {
	(void) magic;

	asm("cli");

	kml_dputs("setting up arch environment...\n");

	// Random arch guff (gdt, idt+pic, tss etc.)
	kmlk_set_arch_tables();
	kmlk_set_interrupt();

	kml_dputs("arch tables done\n");

	//asm("int3");

	// Mem. map
	void* mmctx = kmlk_set_memory(ctx);
	if(!mmctx) kmlk_panic("kmlk_set_memory failed");

	kml_dputs("memory map set, flushing...\n");
	kmlk_mmflush(mmctx);

	// TODO: Append reclaimable pages here before handoff.

	kml_dputs("done arch boot\n");

	kmlk_done_arch(ctx);

	kmlk_start();
}
