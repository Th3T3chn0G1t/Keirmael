// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

#include <kmlk/kernel.h>
#include <kmlk/memory.h>

// TODO: Find a better place to put this.
// TODO: Add arch-specific dump call.
[[noreturn]] void kmlk_panic(const char* s) {
	kml_dputs("PANIC: ");
	kml_dputs(s);
	kmlk_hang();
}

// Arch specific init. calls us here.
void kmlk_start(
		struct kmlk_mem_range* kernel, kml_ptr_t kernel_vbase,
		struct kmlk_mem_range* stack) {

	kml_dputs("Hello, Keirmael!\n");

	kmlk_panic("Reached end of kernel control flow");
}
