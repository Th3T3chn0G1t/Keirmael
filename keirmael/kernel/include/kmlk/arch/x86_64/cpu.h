// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KMLK_X86_64_CPU_H
#define KMLK_X86_64_CPU_H

#include <kml/common.h>

// Static kernel base as defined in `kernel.ld`
#define KMLK_BASE ((kml_ptr_t) 0xFFFFFFFF80000000UL)
#define KMLK_HIGHHALF ((kml_ptr_t) 0xFFFF800000000000UL)

typedef kml_u64_t kmlk_register_t;

enum kmlk_priv {
	KMLK_PRIV0,
	KMLK_PRIV1,
	KMLK_PRIV2,
	KMLK_PRIV3
};

enum kmlk_table {
	KMLK_GDT,
	KMLK_LDT
};

union [[gnu::packed]] kmlk_selector {
	struct [[gnu::packed]] {
		enum kmlk_priv privilege : 2;
		enum kmlk_table table : 1;
		kml_u16_t index : 13;
	};

	kml_u16_t selector;
};

struct [[gnu::packed]] kmlk_flags {
	kml_bool_t carry : 1;
	kml_bool_t reserved0 : 1;
	kml_bool_t parity : 1;
	kml_bool_t reserved1 : 1;
	kml_bool_t adjust : 1;
	kml_bool_t reserved2 : 1;
	kml_bool_t zero : 1;
	kml_bool_t sign : 1;
	kml_bool_t trap : 1;
	kml_bool_t interrupt : 1;
	kml_bool_t direction : 1;
	kml_bool_t overflow : 1;
	enum kmlk_priv privilege : 2;
	kml_bool_t nested_task : 1;
	kml_bool_t reserved3 : 1;
	kml_bool_t resume : 1;
	kml_bool_t v8086 : 1;
	kml_bool_t alignment_check : 1;
	kml_bool_t vinterrupt : 1;
	kml_bool_t vinterrupt_pending : 1;
	kml_bool_t cpuid_usable : 1;

	kml_u64_t reserved4 : 42;
};

struct kmlk_registers {
	union [[gnu::packed]] {
		kmlk_register_t registers[18];

		struct {
			kmlk_register_t rax;
			kmlk_register_t rbx;
			kmlk_register_t rcx;
			kmlk_register_t rdx;

			kmlk_register_t rsi;
			kmlk_register_t rdi;
			kmlk_register_t rbp;
			kmlk_register_t rsp;

			kmlk_register_t r8;
			kmlk_register_t r9;
			kmlk_register_t r10;
			kmlk_register_t r11;
			kmlk_register_t r12;
			kmlk_register_t r13;
			kmlk_register_t r14;
			kmlk_register_t r15;

			kmlk_register_t rip;

			union {
				struct kmlk_flags rflags;
				kmlk_register_t rfl;
			};
		};
	};
};

union [[gnu::packed]] kmlk_cr0 {
	struct [[gnu::packed]] {
		kml_bool_t protected_mode : 1;
		kml_bool_t await_coprocessor : 1;
		kml_bool_t emulate_coprocessor : 1;
		kml_bool_t task_switched : 1;
		kml_bool_t reserved0 : 1;
		kml_bool_t coprocessor_exceptions : 1;
		kml_u16_t reserved1 : 10;
		kml_bool_t write_protect : 1;
		kml_bool_t reserved2 : 1;
		kml_bool_t alignment_check : 1;
		kml_u16_t reserved3 : 10;
		kml_bool_t reserved4 : 1;
		kml_bool_t cache_disable : 1;
		kml_bool_t paging_enable : 1;
		kml_u32_t reserved5;
	};

	kmlk_register_t cr0;
};

union [[gnu::packed]] kmlk_cr3 {
	struct [[gnu::packed]] {
		kml_u8_t reserved0 : 3;
		kml_bool_t table_writethrough : 1;
		kml_bool_t table_cacheable : 1;
		kml_u8_t reserved1 : 7;

		kml_u64_t table_address : 40;
		kml_u16_t reserved2 : 12;
	};

	kmlk_register_t cr3;
};

union [[gnu::packed]] kmlk_cr4 {
	struct [[gnu::packed]] {
		kml_bool_t v8086_virtual_interrupts : 1;
		kml_bool_t protected_mode_virtual_interrupts : 1;
		kml_bool_t privileged_rdts : 1;
		kml_bool_t debug_extensions : 1;
		kml_bool_t page_size_extension : 1;
		kml_bool_t physical_address_extension : 1;
		kml_bool_t machine_check : 1;
		kml_bool_t page_global : 1;
		kml_bool_t performance_monitoring : 1;
		kml_bool_t legacy_sse : 1;
		kml_bool_t simd_fault : 1;
		kml_bool_t restrict_privileged_instructions : 1;
		kml_bool_t level_5_paging : 1;
		kml_u8_t reserved0 : 3;
		kml_bool_t privileged_hidden_segments : 1;
		kml_bool_t pcid : 1;
		kml_bool_t xsave : 1;
		kml_bool_t reserved1 : 1;
		kml_bool_t restrict_privileged_execution : 1;
		kml_bool_t restrict_privileged_access : 1;
		kml_bool_t protection_keys : 1;
		kml_bool_t control_flow_enforcement : 1;
		kml_u64_t reserved2 : 40;
	};

	kmlk_register_t cr4;
};

#endif
