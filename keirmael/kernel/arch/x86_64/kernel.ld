/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>
 */

OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

PHDRS
{
    null PT_NULL FLAGS(0);

    text PT_LOAD FLAGS((1 << 0) | (1 << 2));
    rodata PT_LOAD FLAGS((1 << 2));
    data PT_LOAD FLAGS((1 << 1) | (1 << 2));

    tss PT_LOAD FLAGS((1 << 2));
}

SECTIONS
{
    . = 0xFFFFFFFF80000000;

    /*
     * Place TSS at static address to avoid needing to insert system entries at
     * Runtime.
     */
    .tss : {
        *(.tss .tss.*)
    } :tss

    . += CONSTANT(MAXPAGESIZE);

    .text : {
        *(.text .text.*)
    } :text

    . += CONSTANT(MAXPAGESIZE);

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    . += CONSTANT(MAXPAGESIZE);

    .data : {
        *(.data .data.*)
    } :data

    . += CONSTANT(MAXPAGESIZE);

    .bss : {
        *(COMMON)
        *(.bss .bss.*)
    } :data
}
