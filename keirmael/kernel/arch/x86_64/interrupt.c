// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

#include <kmlk/kernel.h>

#include <kmlk/arch/x86_64/vectors.h>

struct kmlk_interrupt_frame kmlk_exception_frame;

[[noreturn]] [[gnu::no_caller_saved_registers]]
void kmlk_interrupt_stub(void) {
	kml_dputs("Received interrupt!");
	kmlk_hang();
}
