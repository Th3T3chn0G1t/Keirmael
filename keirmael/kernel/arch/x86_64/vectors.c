// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/common.h>

#include <kmlk/arch/x86_64/vectors.h>
#include <kmlk/arch/x86_64/tables.h>

// NOTE: These cursed macros make it easier to write our mess after all this.
#define _Irq(n) \
	[[gnu::naked]] void kmlk_irq##n(void) { \
		_Base("movl $0, %[error_code]\n", KMLK_IRQ0 + n); \
	}

#define _Exc(vector) \
	[[gnu::naked]] void kmlk_exc##vector(void) { \
		_Base("popq %[error_code]\n", vector); \
	}

#define _Base(ec, vec) \
	asm("cli\n" \
		/* Exception Entry Frame */ \
		ec \
		"popq %[rip]\n" \
		"popq %[cs]\n" \
		"popq %[rflags]\n" \
		"popq %[rsp]\n" \
		"popq %[ss]\n" \
		\
		/* Exception Frame */ \
		"movl %[exception_vector], %[vector]\n" \
		"movq %%rax, %[rax]\n" \
		"movq %%rbx, %[rbx]\n" \
		"movq %%rcx, %[rcx]\n" \
		"movq %%rdx, %[rdx]\n" \
		"movq %%rsi, %[rsi]\n" \
		"movq %%rdi, %[rdi]\n" \
		"movq %%rbp, %[rbp]\n" \
		"movq %%r8, %[r8]\n" \
		"movq %%r9, %[r9]\n" \
		"movq %%r10, %[r10]\n" \
		"movq %%r11, %[r11]\n" \
		"movq %%r12, %[r12]\n" \
		"movq %%r13, %[r13]\n" \
		"movq %%r14, %[r14]\n" \
		"movq %%r15, %[r15]\n" \
		\
		".extern kmlk_interrupt_stub\n" \
		"callq kmlk_interrupt_stub\n" \
		"pushq %[ss]\n" \
		"pushq %[rsp]\n" \
		"pushq %[rflags]\n" \
		"pushq %[cs]\n" \
		"pushq %[rip]\n" \
		"iretq\n" \
		: \
		/* Exception Entry Frame */ \
		[error_code] "=m" (kmlk_exception_frame.error_code), \
		[rip] "=m" (kmlk_exception_frame.registers.rip), \
		[cs] "=m" (kmlk_exception_frame.cs), \
		[rflags] "=m" (kmlk_exception_frame.registers.rflags), \
		[rsp] "=m" (kmlk_exception_frame.registers.rsp), \
		[ss] "=m" (kmlk_exception_frame.ss), \
		\
		/* Exception Frame */ \
		[vector] "=m" (kmlk_exception_frame.vector), \
		[rax] "=m" (kmlk_exception_frame.registers.rax), \
		[rbx] "=m" (kmlk_exception_frame.registers.rbx), \
		[rcx] "=m" (kmlk_exception_frame.registers.rcx), \
		[rdx] "=m" (kmlk_exception_frame.registers.rdx), \
		[rsi] "=m" (kmlk_exception_frame.registers.rsi), \
		[rdi] "=m" (kmlk_exception_frame.registers.rdi), \
		[rbp] "=m" (kmlk_exception_frame.registers.rbp), \
		[r8] "=m" (kmlk_exception_frame.registers.r8), \
		[r9] "=m" (kmlk_exception_frame.registers.r9), \
		[r10] "=m" (kmlk_exception_frame.registers.r10), \
		[r11] "=m" (kmlk_exception_frame.registers.r11), \
		[r12] "=m" (kmlk_exception_frame.registers.r12), \
		[r13] "=m" (kmlk_exception_frame.registers.r13), \
		[r14] "=m" (kmlk_exception_frame.registers.r14), \
		[r15] "=m" (kmlk_exception_frame.registers.r15) \
		: [exception_vector] "n" (vec) :)

#define _Vecset_impl(i, id, ist, type, priv) \
    do { \
        kmlk_idt[KMLK_IRQ0 + i] = (struct kmlk_idt_entry) { \
            ((kml_ptr_t) id) & 0xFFFF, \
            kmlk_selectors[KMLK_GDT_INDEX_CODE], \
            (ist), \
            0, \
            type, \
            KML_FALSE, \
            (priv), \
            KML_TRUE, \
            ((kml_ptr_t) id) >> 16,\
            0 \
        }; \
    } while(0)

#define _Irqset(i) \
	_Vecset_impl(i, kmlk_irq##i, KMLK_IST_NONE, KMLK_GATE_INTERRUPT, KMLK_PRIV0)

#define _Excset(vec) \
	_Vecset_impl(vec, kmlk_exc##vec, KMLK_IST_NONE, KMLK_GATE_TRAP, KMLK_PRIV0)

_Exc(KMLK_EXC_DIV0)
_Exc(KMLK_EXC_TRAP)
_Exc(KMLK_EXC_NMI)
_Exc(KMLK_EXC_BREAKPOINT)
_Exc(KMLK_EXC_OVERFLOW)
_Exc(KMLK_EXC_BOUND_RANGE_EXCEEDED)
_Exc(KMLK_EXC_INVALID_OPCODE)
_Exc(KMLK_EXC_DEVICE_NOT_AVAILABLE)
_Exc(KMLK_EXC_DOUBLE_FAULT)
_Exc(KMLK_EXC_COPROCESSOR_SEGMENT_OVERRUN)
_Exc(KMLK_EXC_INVALID_TSS)
_Exc(KMLK_EXC_SEGMENT_NOT_PRESENT)
_Exc(KMLK_EXC_STACK_SEGMENT_FAULT)
_Exc(KMLK_EXC_GENERAL_PROTECTION_FAULT)
_Exc(KMLK_EXC_PAGE_FAULT)
_Exc(KMLK_EXC_X87_FLOATING_POINT)
_Exc(KMLK_EXC_ALIGNMENT_CHECK)
_Exc(KMLK_EXC_MACHINE_CHECK)
_Exc(KMLK_EXC_SIMD_FLOATING_POINT)
_Exc(KMLK_EXC_VIRTUALIZATION)
_Exc(KMLK_EXC_CONTROL_PROTECTION)
_Exc(KMLK_EXC_HYPERVISOR_INJECTION)
_Exc(KMLK_EXC_VMM_COMMUNICATION)
_Exc(KMLK_EXC_SECURITY)

// 223 IRQs.
_Irq(0) _Irq(1) _Irq(2) _Irq(3) _Irq(4) _Irq(5) _Irq(6) _Irq(7) _Irq(8) _Irq(9)
_Irq(10) _Irq(11) _Irq(12) _Irq(13) _Irq(14) _Irq(15) _Irq(16) _Irq(17)
_Irq(18) _Irq(19) _Irq(20) _Irq(21) _Irq(22) _Irq(23) _Irq(24) _Irq(25)
_Irq(26) _Irq(27) _Irq(28) _Irq(29) _Irq(30) _Irq(31) _Irq(32) _Irq(33)
_Irq(34) _Irq(35) _Irq(36) _Irq(37) _Irq(38) _Irq(39) _Irq(40) _Irq(41)
_Irq(42) _Irq(43) _Irq(44) _Irq(45) _Irq(46) _Irq(47) _Irq(48) _Irq(49)
_Irq(50) _Irq(51) _Irq(52) _Irq(53) _Irq(54) _Irq(55) _Irq(56) _Irq(57)
_Irq(58) _Irq(59) _Irq(60) _Irq(61) _Irq(62) _Irq(63) _Irq(64) _Irq(65)
_Irq(66) _Irq(67) _Irq(68) _Irq(69) _Irq(70) _Irq(71) _Irq(72) _Irq(73)
_Irq(74) _Irq(75) _Irq(76) _Irq(77) _Irq(78) _Irq(79) _Irq(80) _Irq(81)
_Irq(82) _Irq(83) _Irq(84) _Irq(85) _Irq(86) _Irq(87) _Irq(88) _Irq(89)
_Irq(90) _Irq(91) _Irq(92) _Irq(93) _Irq(94) _Irq(95) _Irq(96) _Irq(97)
_Irq(98) _Irq(99) _Irq(100) _Irq(101) _Irq(102) _Irq(103) _Irq(104) _Irq(105)
_Irq(106) _Irq(107) _Irq(108) _Irq(109) _Irq(110) _Irq(111) _Irq(112) _Irq(113)
_Irq(114) _Irq(115) _Irq(116) _Irq(117) _Irq(118) _Irq(119) _Irq(120) _Irq(121)
_Irq(122) _Irq(123) _Irq(124) _Irq(125) _Irq(126) _Irq(127) _Irq(128) _Irq(129)
_Irq(130) _Irq(131) _Irq(132) _Irq(133) _Irq(134) _Irq(135) _Irq(136) _Irq(137)
_Irq(138) _Irq(139) _Irq(140) _Irq(141) _Irq(142) _Irq(143) _Irq(144) _Irq(145)
_Irq(146) _Irq(147) _Irq(148) _Irq(149) _Irq(150) _Irq(151) _Irq(152) _Irq(153)
_Irq(154) _Irq(155) _Irq(156) _Irq(157) _Irq(158) _Irq(159) _Irq(160) _Irq(161)
_Irq(162) _Irq(163) _Irq(164) _Irq(165) _Irq(166) _Irq(167) _Irq(168) _Irq(169)
_Irq(170) _Irq(171) _Irq(172) _Irq(173) _Irq(174) _Irq(175) _Irq(176) _Irq(177)
_Irq(178) _Irq(179) _Irq(180) _Irq(181) _Irq(182) _Irq(183) _Irq(184) _Irq(185)
_Irq(186) _Irq(187) _Irq(188) _Irq(189) _Irq(190) _Irq(191) _Irq(192) _Irq(193)
_Irq(194) _Irq(195) _Irq(196) _Irq(197) _Irq(198) _Irq(199) _Irq(200) _Irq(201)
_Irq(202) _Irq(203) _Irq(204) _Irq(205) _Irq(206) _Irq(207) _Irq(208) _Irq(209)
_Irq(210) _Irq(211) _Irq(212) _Irq(213) _Irq(214) _Irq(215) _Irq(216) _Irq(217)
_Irq(218) _Irq(219) _Irq(220) _Irq(221) _Irq(222) _Irq(223)

// Install default interrupt vector handlers.
void kmlk_set_vectors(void) {
	_Excset(KMLK_EXC_DIV0);
	_Excset(KMLK_EXC_TRAP);
	_Excset(KMLK_EXC_NMI);
	_Excset(KMLK_EXC_BREAKPOINT);
	_Excset(KMLK_EXC_OVERFLOW);
	_Excset(KMLK_EXC_BOUND_RANGE_EXCEEDED);
	_Excset(KMLK_EXC_INVALID_OPCODE);
	_Excset(KMLK_EXC_DEVICE_NOT_AVAILABLE);
	_Excset(KMLK_EXC_DOUBLE_FAULT);
	_Excset(KMLK_EXC_COPROCESSOR_SEGMENT_OVERRUN);
	_Excset(KMLK_EXC_INVALID_TSS);
	_Excset(KMLK_EXC_SEGMENT_NOT_PRESENT);
	_Excset(KMLK_EXC_STACK_SEGMENT_FAULT);
	_Excset(KMLK_EXC_GENERAL_PROTECTION_FAULT);
	_Excset(KMLK_EXC_PAGE_FAULT);
	_Excset(KMLK_EXC_X87_FLOATING_POINT);
	_Excset(KMLK_EXC_ALIGNMENT_CHECK);
	_Excset(KMLK_EXC_MACHINE_CHECK);
	_Excset(KMLK_EXC_SIMD_FLOATING_POINT);
	_Excset(KMLK_EXC_VIRTUALIZATION);
	_Excset(KMLK_EXC_CONTROL_PROTECTION);
	_Excset(KMLK_EXC_HYPERVISOR_INJECTION);
	_Excset(KMLK_EXC_VMM_COMMUNICATION);
	_Excset(KMLK_EXC_SECURITY);

	// Set all IRQ vectors.
	_Irqset(0); _Irqset(1); _Irqset(2); _Irqset(3); _Irqset(4); _Irqset(5);
	_Irqset(6); _Irqset(7); _Irqset(8); _Irqset(9); _Irqset(10); _Irqset(11);
	_Irqset(12); _Irqset(13); _Irqset(14); _Irqset(15); _Irqset(16);
	_Irqset(17); _Irqset(18); _Irqset(19); _Irqset(20); _Irqset(21);
	_Irqset(22); _Irqset(23); _Irqset(24); _Irqset(25); _Irqset(26);
	_Irqset(27); _Irqset(28); _Irqset(29); _Irqset(30); _Irqset(31);
	_Irqset(32); _Irqset(33); _Irqset(34); _Irqset(35); _Irqset(36);
	_Irqset(37); _Irqset(38); _Irqset(39); _Irqset(40); _Irqset(41);
	_Irqset(42); _Irqset(43); _Irqset(44); _Irqset(45); _Irqset(46);
	_Irqset(47); _Irqset(48); _Irqset(49); _Irqset(50); _Irqset(51);
	_Irqset(52); _Irqset(53); _Irqset(54); _Irqset(55); _Irqset(56);
	_Irqset(57); _Irqset(58); _Irqset(59); _Irqset(60); _Irqset(61);
	_Irqset(62); _Irqset(63); _Irqset(64); _Irqset(65); _Irqset(66);
	_Irqset(67); _Irqset(68); _Irqset(69); _Irqset(70); _Irqset(71);
	_Irqset(72); _Irqset(73); _Irqset(74); _Irqset(75); _Irqset(76);
	_Irqset(77); _Irqset(78); _Irqset(79); _Irqset(80); _Irqset(81);
	_Irqset(82); _Irqset(83); _Irqset(84); _Irqset(85); _Irqset(86);
	_Irqset(87); _Irqset(88); _Irqset(89); _Irqset(90); _Irqset(91);
	_Irqset(92); _Irqset(93); _Irqset(94); _Irqset(95); _Irqset(96);
	_Irqset(97); _Irqset(98); _Irqset(99); _Irqset(100); _Irqset(101);
	_Irqset(102); _Irqset(103); _Irqset(104); _Irqset(105); _Irqset(106);
	_Irqset(107); _Irqset(108); _Irqset(109); _Irqset(110); _Irqset(111);
	_Irqset(112); _Irqset(113); _Irqset(114); _Irqset(115); _Irqset(116);
	_Irqset(117); _Irqset(118); _Irqset(119); _Irqset(120); _Irqset(121);
	_Irqset(122); _Irqset(123); _Irqset(124); _Irqset(125); _Irqset(126);
	_Irqset(127); _Irqset(128); _Irqset(129); _Irqset(130); _Irqset(131);
	_Irqset(132); _Irqset(133); _Irqset(134); _Irqset(135); _Irqset(136);
	_Irqset(137); _Irqset(138); _Irqset(139); _Irqset(140); _Irqset(141);
	_Irqset(142); _Irqset(143); _Irqset(144); _Irqset(145); _Irqset(146);
	_Irqset(147); _Irqset(148); _Irqset(149); _Irqset(150); _Irqset(151);
	_Irqset(152); _Irqset(153); _Irqset(154); _Irqset(155); _Irqset(156);
	_Irqset(157); _Irqset(158); _Irqset(159); _Irqset(160); _Irqset(161);
	_Irqset(162); _Irqset(163); _Irqset(164); _Irqset(165); _Irqset(166);
	_Irqset(167); _Irqset(168); _Irqset(169); _Irqset(170); _Irqset(171);
	_Irqset(172); _Irqset(173); _Irqset(174); _Irqset(175); _Irqset(176);
	_Irqset(177); _Irqset(178); _Irqset(179); _Irqset(180); _Irqset(181);
	_Irqset(182); _Irqset(183); _Irqset(184); _Irqset(185); _Irqset(186);
	_Irqset(187); _Irqset(188); _Irqset(189); _Irqset(190); _Irqset(191);
	_Irqset(192); _Irqset(193); _Irqset(194); _Irqset(195); _Irqset(196);
	_Irqset(197); _Irqset(198); _Irqset(199); _Irqset(200); _Irqset(201);
	_Irqset(202); _Irqset(203); _Irqset(204); _Irqset(205); _Irqset(206);
	_Irqset(207); _Irqset(208); _Irqset(209); _Irqset(210); _Irqset(211);
	_Irqset(212); _Irqset(213); _Irqset(214); _Irqset(215); _Irqset(216);
	_Irqset(217); _Irqset(218); _Irqset(219); _Irqset(220); _Irqset(221);
	_Irqset(222); _Irqset(223);
}
