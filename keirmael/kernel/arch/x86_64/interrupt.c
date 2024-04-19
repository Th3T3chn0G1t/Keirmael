// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

#include <kmlk/kernel.h>

#include <kmlk/arch/x86_64/ports.h>
#include <kmlk/arch/x86_64/vectors.h>
#include <kmlk/arch/x86_64/interrupt.h>

struct kmlk_interrupt_frame kmlk_exception_frame;

[[gnu::no_caller_saved_registers]] void kmlk_interrupt_stub(void) {
	kml_u64_t vec = kmlk_exception_frame.vector;

	switch(vec) {
		case KMLK_EXC_PAGE_FAULT: {
			kmlk_register_t cr2;
			asm("movq %%cr2, %[cr2]" : [cr2] "=a" (cr2) :: );
			KML_DLOG("page fault @ linear address $X", cr2);

			struct kmlk_page_error* pe =
					&kmlk_exception_frame.error_code.page_fault;

			if(pe->present) {
				const char* op = pe->write ? "write" : "read";
				if(pe->instruction_fetch) op = "exec";

				KML_DLOG("page protection violation on $S", op);
			}
			else KML_DLOG("fault on missing page");

			break;
		}

		default: KML_DLOG("received interrupt $X", vec);
	}

	kmlk_hang();
}

void kmlk_set_interrupt(void) {
	// Mask out all interrupts.
	kmlk_outb(KMLK_PIC1 + 1, 0xFF);
	kmlk_outb(KMLK_PIC2 + 1, 0xFF);

	// TODO: APIC once we have ACPI.

	asm("sti");
}
