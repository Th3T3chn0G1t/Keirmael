// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KMLK_X86_64_TABLES_H
#define KMLK_X86_64_TABLES_H

// CPU tables that x86_64 wants -- TSS/GDT etc.

#include <kml/common.h>

#include <kmlk/arch/x86_64/cpu.h>
#include <kmlk/arch/x86_64/vectors.h>

enum kmlk_gdt_type {
	KMLK_GDT_SYSTEM,
	KMLK_GDT_CODE_DATA
};

struct kmlk_gdt_entry {
	union {
		struct [[gnu::packed]] {
			kml_u16_t limit_low;
			kml_u32_t base_low : 24;

			union {
				struct [[gnu::packed]] {
					kml_bool_t accessed : 1;

					// The meaning of these flags change depending on the
					// Code/data mode of the GDT entry.
					enum {
						// Data writeable/unwriteable
						KMLK_GDT_WRITEABLE,
						KMLK_GDT_UNWRITEABLE,

						// Code readable/unreadable.
						KMLK_GDT_READABLE = 0,
						KMLK_GDT_UNREADABLE
					} readable_writeable : 1;
					enum {
						// Data direction.
						KMLK_GDT_DIRECTION_DOWN,
						KMLK_GDT_DIRECTION_UP,

						// Code conforming privilege.
						KMLK_GDT_CONFORMING_RESTRICTED = 0,
						KMLK_GDT_CONFORMING_UNRESTRICTED
					} direction_conforming : 1;

					kml_bool_t executable : 1;
					enum kmlk_gdt_type type : 1;
					enum kmlk_priv privilege : 2;
					kml_bool_t present : 1;
				} code_data_access;

				struct [[gnu::packed]] {
					enum {
						KMLK_GDT_SYSTEM_LDT = 2,

						KMLK_GDT_SYSTEM_TSS_IDLE = 9,
						KMLK_GDT_SYSTEM_TSS_BUSY = 11
					} system_type : 4;

					enum kmlk_gdt_type type : 1;
					enum kmlk_priv privilege : 2;
					kml_bool_t present : 1;
				} system_access;
			};

			kml_u8_t limit_high : 4;

			kml_bool_t reserved : 1;
			kml_bool_t long_mode : 1;

			enum {
				KMLK_GDT_WIDTH_16,
				KMLK_GDT_WIDTH_32,

				KMLK_GDT_WIDTH_64 = 0
			} width : 1;

			enum {
				KMLK_GDT_GRANULARITY_1B,
				KMLK_GDT_GRANULARITY_4KIB
			} granularity : 1;

			kml_u8_t base_high;
		} entry;

		struct [[gnu::packed]] {
			kml_u32_t base_high;
			kml_u32_t reserved;
		} system_entry_high;
	};
};

struct [[gnu::packed]] kmlk_gdt_pointer {
	kml_u16_t limit;
	const struct kmlk_gdt_entry* base;
};

enum kmlk_gdt_index {
	KMLK_GDT_INDEX_NULL,
	KMLK_GDT_INDEX_CODE,
	KMLK_GDT_INDEX_DATA,
	KMLK_GDT_INDEX_TSS_BASE,
	KMLK_GDT_INDEX_TSS_HIGH,

	KMLK_GDT_INDEX_COUNT
};

enum kmlk_ist {
	KMLK_IST_NONE,
	KMLK_IST1,
	KMLK_IST2,
	KMLK_IST3,
	KMLK_IST4,
	KMLK_IST5,
	KMLK_IST6,
	KMLK_IST7,

	KMLK_IST_COUNT = KMLK_IST7
};

// TODO: I/O Permission Bit Map
// TODO: TSS Interrupt Stack Table for IDT entries `sysenter`
struct [[gnu::packed]] kmlk_tss {
	kml_u32_t reserved0;
	kml_ptr_t rsp[3];
	kml_ptr_t reserved1[2];
	kml_ptr_t ist[KMLK_IST_COUNT];
	kml_ptr_t reserved2[2];
	kml_u16_t reserved3;
	kml_u16_t iopb_offset;
};

enum kmlk_gate_type {
	KMLK_GATE_INTERRUPT = 0xE,
	KMLK_GATE_TRAP = 0xF
};

struct [[gnu::packed]] kmlk_idt_entry {
	kml_u16_t offset_low;

	union kmlk_selector selector;

	kml_u8_t ist : 3;
	kml_u8_t reserved0 : 5;

	enum kmlk_gate_type gate_type : 4;
	kml_bool_t reserved1 : 1;
	enum kmlk_priv privilege : 2;
	kml_bool_t present : 1;

	kml_u64_t offset_high : 48;

	kml_u32_t reserved2;
};

struct [[gnu::packed]] kmlk_idt_pointer {
	kml_u16_t size;
	struct kmlk_idt_entry* offset;
};

extern struct kmlk_tss kmlk_tss;
extern const struct kmlk_gdt_entry kmlk_gdt[KMLK_GDT_INDEX_COUNT];
extern const union kmlk_selector kmlk_selectors[KMLK_GDT_INDEX_COUNT];

extern struct kmlk_idt_entry kmlk_idt[KMLK_VECTOR_COUNT];

void kmlk_set_arch_tables(void);

void kmlk_set_vectors(void);

#endif
