// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

#include <kmlk/arch/x86_64/ports.h>
#include <kmlk/arch/x86_64/vectors.h>
#include <kmlk/arch/x86_64/interrupt.h>

struct kmlk_interrupt_frame kmlk_exception_frame;

[[gnu::no_caller_saved_registers]] void kmlk_interrupt_stub(void) {
	kml_dputf(
			"received interrupt $X\n",
			(kml_u64_t) kmlk_exception_frame.vector);
}

void kmlk_set_interrupt(void) {
	// Mask out all interrupts.
	kmlk_outb(KMLK_PIC1 + 1, 0xFF);
	kmlk_outb(KMLK_PIC2 + 1, 0xFF);

	// TODO: APIC once we have ACPI.

	asm("sti");
}
