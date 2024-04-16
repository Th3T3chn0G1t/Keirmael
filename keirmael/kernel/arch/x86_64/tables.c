// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kmlk/arch/x86_64/cpu.h>
#include <kmlk/arch/x86_64/tables.h>

// TODO: Make a nice way to poke at this once we have SMP to add TSS for each
//		 Running task. Probably shouldn't just have a single global TSS once we
//		 Get there anyway.
struct kmlk_tss kmlk_tss [[gnu::section("tss")]] = { 0 };

const struct kmlk_gdt_entry kmlk_gdt[] = {
		[KMLK_GDT_INDEX_NULL] = { 0 },
		[KMLK_GDT_INDEX_CODE] = { .entry = {
				0xFFFF, 0,
				{ .code_data_access = {
						KML_TRUE,
						KMLK_GDT_UNREADABLE,
						KMLK_GDT_CONFORMING_RESTRICTED,
						KML_TRUE,
						KMLK_GDT_CODE_DATA,
						KMLK_PRIV0,
						KML_TRUE
				} },
				0xF,
				KML_FALSE,
				KML_TRUE,
				KMLK_GDT_WIDTH_64,
				KMLK_GDT_GRANULARITY_4KIB,
				0
		} },

		[KMLK_GDT_INDEX_DATA] = { .entry = {
				0xFFFF, 0,
				{ .code_data_access = {
						KML_TRUE,
						KMLK_GDT_UNWRITEABLE,
						KMLK_GDT_DIRECTION_DOWN,
						KML_FALSE,
						KMLK_GDT_CODE_DATA,
						KMLK_PRIV0,
						KML_TRUE
				} },
				0xF,
				KML_FALSE,
				KML_TRUE,
				KMLK_GDT_WIDTH_64,
				KMLK_GDT_GRANULARITY_4KIB,
				0
		} },

		/*
		 * TSS section is placed at kernel base address for x86_64 to avoid
		 * Needing to write these entries at runtime.
		 */
		[KMLK_GDT_INDEX_TSS_BASE] = { .entry = {
				sizeof(kmlk_tss) & 0xFFFF,
				KMLK_BASE & 0xFFFFFF,
				{ .system_access = {
						KMLK_GDT_SYSTEM_TSS_IDLE,
						KMLK_GDT_SYSTEM,
						KMLK_PRIV0,
						KML_TRUE
				} },
				(sizeof(kmlk_tss) >> 2) & 0xF,
				KML_FALSE,
				KML_TRUE,
				KMLK_GDT_WIDTH_64,
				KMLK_GDT_GRANULARITY_1B,
				(KMLK_BASE >> 24) & 0xFF
		} },
		[KMLK_GDT_INDEX_TSS_HIGH] = { .system_entry_high = {
				(kml_u32_t) (KMLK_BASE >> 32), 0
		} }
};

const union kmlk_selector kmlk_selectors[] = {
		[KMLK_GDT_INDEX_NULL] = { {
			KMLK_PRIV0, KMLK_GDT, KMLK_GDT_INDEX_NULL
		} },
		[KMLK_GDT_INDEX_CODE] = { {
			KMLK_PRIV0, KMLK_GDT, KMLK_GDT_INDEX_CODE
		} },
		[KMLK_GDT_INDEX_DATA] = { {
			KMLK_PRIV0, KMLK_GDT, KMLK_GDT_INDEX_DATA
		} },
		[KMLK_GDT_INDEX_TSS_BASE] = { {
			KMLK_PRIV0, KMLK_GDT, KMLK_GDT_INDEX_TSS_BASE
		} }
};

const struct kmlk_gdt_pointer kmlk_gdt_pointer = {
		sizeof(kmlk_gdt) - 1, kmlk_gdt
};

void kmlk_set_arch_tables(void) {
	asm(
		// Load GDT.
		"lgdtq %[gdt]\n"

		// Fill in segment regs..
		"movw %[data], %%ax\n"
		"movw %%ax, %%ds\n"
		"movw %%ax, %%es\n"
		"movw %%ax, %%fs\n"
		"movw %%ax, %%gs\n"
		"movw %%ax, %%ss\n"
		"pushq %[code]\n"
		"lea .done_reload_cs(%%rip), %%rax\n"
		"pushq %%rax\n"
		"lretq\n"
		".done_reload_cs:\n"

		// Load TSS.
		"ltr %[tss]\n"

		: : [code] "n" (kmlk_selectors[KMLK_GDT_INDEX_CODE]),
			[data] "n" (kmlk_selectors[KMLK_GDT_INDEX_DATA]),
			[tss] "m" (kmlk_selectors[KMLK_GDT_INDEX_TSS_BASE]),
			[gdt] "m" (kmlk_gdt_pointer)

		: "rax", "rbx");
}
