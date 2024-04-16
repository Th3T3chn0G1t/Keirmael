// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KMLK_X86_64_CPU_H
#define KMLK_X86_64_CPU_H

#include <kml/common.h>

// Static kernel base as defined in `kernel.ld`
#define KMLK_BASE ((kml_ptr_t) 0xFFFFFFFF80000000UL)

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

#endif
