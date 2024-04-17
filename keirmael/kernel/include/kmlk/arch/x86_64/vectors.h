// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KMLK_X86_64_VECTORS_H
#define KMLK_X86_64_VECTORS_H

// Define vectors and handler inputs.

#include <kml/common.h>

#include <kmlk/arch/x86_64/cpu.h>

// KMLK_IRQ0 + [0, KMLK_IRQ_MAX]
#define KMLK_IRQ_MAX (223)

enum kmlk_vector {
	KMLK_EXC_DIV0,
	KMLK_EXC_TRAP,
	KMLK_EXC_NMI,
	KMLK_EXC_BREAKPOINT,
	KMLK_EXC_OVERFLOW,
	KMLK_EXC_BOUND_RANGE_EXCEEDED,
	KMLK_EXC_INVALID_OPCODE,
	KMLK_EXC_DEVICE_NOT_AVAILABLE,
	KMLK_EXC_DOUBLE_FAULT,
	KMLK_EXC_COPROCESSOR_SEGMENT_OVERRUN,
	KMLK_EXC_INVALID_TSS,
	KMLK_EXC_SEGMENT_NOT_PRESENT,
	KMLK_EXC_STACK_SEGMENT_FAULT,
	KMLK_EXC_GENERAL_PROTECTION_FAULT,
	KMLK_EXC_PAGE_FAULT,
	KMLK_EXC_RESERVED0,
	KMLK_EXC_X87_FLOATING_POINT,
	KMLK_EXC_ALIGNMENT_CHECK,
	KMLK_EXC_MACHINE_CHECK,
	KMLK_EXC_SIMD_FLOATING_POINT,
	KMLK_EXC_VIRTUALIZATION,
	KMLK_EXC_CONTROL_PROTECTION,
	KMLK_EXC_RESERVED1,
	KMLK_EXC_RESERVED2,
	KMLK_EXC_RESERVED3,
	KMLK_EXC_RESERVED4,
	KMLK_EXC_RESERVED5,
	KMLK_EXC_RESERVED6,
	KMLK_EXC_HYPERVISOR_INJECTION,
	KMLK_EXC_VMM_COMMUNICATION,
	KMLK_EXC_SECURITY,
	KMLK_EXC_RESERVED7,

	KMLK_IRQ0,

	KMLK_VECTOR_COUNT = 256
};

struct [[gnu::packed]] kmlk_segment_error {
	kml_bool_t external : 1;

	enum {
		KMLK_SEGMENT_ERROR_GDT = 0b00,
		KMLK_SEGMENT_ERROR_IDT = 0b01,
		KMLK_SEGMENT_ERROR_LDT = 0b10,
		KMLK_SEGMENT_ERROR_IDT2 = 0b11
	} selector : 2;

	kml_u16_t index : 13;
	kml_u16_t pad0;
};

struct [[gnu::packed]] kmlk_page_error {
	// true: Page-protection violation; false: Non-present page
	kml_bool_t present : 1;
	// true: Write; false: Read
	kml_bool_t write : 1;
	// true: CPL == 3;
	kml_bool_t user : 1;
	// true: Page directory sets reserved bit;
	kml_bool_t reserved_write : 1;
	// true: Fault caused by instruction fetch;
	kml_bool_t instruction_fetch : 1;

	// TODO: "The PKRU register (for user-mode accesses) or PKRS MSR
	//  	 (for supervisor-mode accesses) specifies the protection key
	//  	 rights."
	// true: Fault caused by protection-key violation;
	kml_bool_t protection_key : 1;

	// true: Fault caused by shadow stack access;
	kml_bool_t shadow_stack : 1;

	kml_u8_t reserved0;

	// true: Fault caused by SGX violation;
	kml_bool_t sgx_violation : 1;

	kml_u16_t reserved1;
};

struct kmlk_interrupt_frame {
	enum kmlk_vector vector;
	struct kmlk_registers registers;

	struct [[gnu::packed]] {
		union [[gnu::packed]] {
			kml_u32_t error_code;

			// Always 0
			kml_u32_t double_fault;

			// `ss` selector
			struct kmlk_segment_error invalid_tss;
			// Segment selector
			struct kmlk_segment_error segment_not_present;
			// `ss` selector
			struct kmlk_segment_error stack_segment_fault;
			// Segment selector else 0
			struct kmlk_segment_error general_protection_fault;

			struct kmlk_page_error page_fault;

			// Always 0
			kml_u32_t alignment_check;
			// `VM_EXIT` code
			kml_u32_t vmm_communication;
			// Always 1
			kml_u32_t security;
		};

		kml_u32_t pad0;
	} error_code;

	struct [[gnu::packed]] {
		union kmlk_selector ss;
		kml_u64_t pad1 : 48;
	} ss;

	struct [[gnu::packed]] {
		union kmlk_selector cs;
		kml_u64_t pad1 : 48;
	} cs;
};

// TODO: When we get to SMP make this CPU local.
// Please never modify this.
extern struct kmlk_interrupt_frame kmlk_exception_frame;

[[gnu::no_caller_saved_registers]] void kmlk_interrupt_stub(void);

#endif
