// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

#include <kmlk/kernel.h>
#include <kmlk/memory.h>

// TODO: Find a better place to put this.
// TODO: Add arch-specific dump call.
[[noreturn]] void kmlk_panic(const char* s) {
	KML_DLOG("PANIC: $S", s);
	kmlk_hang();
}

// Arch specific init. calls us here.
[[noreturn]] void kmlk_start(void* mmctx) {
	KML_DLOG("hello, keirmael!");

	void* mmcopy = kmlk_pcalloc();
	if(!mmcopy) kmlk_panic("kmlk_pcalloc: out of memory");

	KML_DLOG("alloc'd mmcopy @ $X", mmcopy);

	kml_memcpy(mmcopy, mmctx, KMLK_PAGE);
	kmlk_mmflush(mmcopy);

	/*
	enum kml_result res = kmlk_mmcopy_global(&mmtest, mmctx);
	if(res) {
		kml_presult("kmlk_mmcopy_global", res);
		kmlk_panic("failed to create new mmctx");
	}

	for(kml_size_t i = 0; i < KMLK_PAGE; ++i) {
		if(((char*) mmctx)[i] != ((char*) mmtest)[i]) KML_DLOG("!!mm[$X]", i);
	}*/


	kmlk_panic("reached end of kernel control flow\n");
}
