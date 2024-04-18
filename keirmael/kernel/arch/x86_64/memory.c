// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kmlk/memory.h>

#include <kmlk/arch/x86_64/tables.h>

// x86_64 `mmctx` is a pointer to the top level page entry.

static kml_size_t kmlk_pidx(kml_ptr_t vaddr, kmlk_pt_lvl_t lvl) {
	return (vaddr >> (12 + 9 * lvl)) & 0b111111111;
}

enum kml_result kmlk_mmap(
		void** mmctx, kmlk_paddr_t paddr, kml_ptr_t vaddr,
		enum kmlk_mem_prot prot) {

	if(vaddr & (KMLK_PAGE - 1)) return KML_E_ALIGN;
	if(paddr & (KMLK_PAGE - 1)) return KML_E_ALIGN;

	if(!*mmctx && !(*mmctx = kmlk_pcalloc())) return KML_E_OOM;

	union kmlk_pt_entry* top = *mmctx;
	union kmlk_pt_entry* table = &top[kmlk_pidx(vaddr, 3)];

	for(kml_size_t i = 0; i < 4; ++i) {
		if(!table->present) {
			void* p = kmlk_pcalloc();
			// TODO: This leaks unnecessarily allocated leaves.
			if(!p) return KML_E_OOM;

			kml_ptr_t address = (kml_ptr_t) p >> 12;
			table->address = address;

			table->present = KML_TRUE;
			table->writeable = KML_TRUE;
		}

		kml_ptr_t addr = table->address << 12;
		table = &((union kmlk_pt_entry*) addr)[kmlk_pidx(vaddr, 3 - i)];
	}

	table->address = paddr >> 12;
	table->present = KML_TRUE;
	table->writeable = KML_TRUE;

	return KML_OK;
}

void kmlk_mflush(void* mmctx) {
	union kmlk_cr3 cr3 = { 0 };

	cr3.table_address = (kml_ptr_t) mmctx >> 12;

	asm("movq %%rax, %%cr3" :: [value] "a" (cr3.cr3) : );
}
