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

	KML_DLOG("setting up arch environment...");

	// Random arch guff (gdt, idt+pic, tss etc.)
	kmlk_set_arch_tables();
	kmlk_set_interrupt();

	KML_DLOG("arch tables done");

	//asm("int3");

	// Mem. map
	void* mmctx = kmlk_set_memory(ctx);
	if(!mmctx) kmlk_panic("kmlk_set_memory failed");

	KML_DLOG("memory map set, flushing...");
	kmlk_mmflush(mmctx);

	KML_DLOG("done arch boot");

	kmlk_done_arch(ctx);

	kmlk_start();
}
