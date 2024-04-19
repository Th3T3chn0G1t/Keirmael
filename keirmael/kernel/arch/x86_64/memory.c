// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

#include <kmlk/memory.h>

#include <kmlk/arch/x86_64/tables.h>

#include <ultra_protocol.h>

static struct kmlk_pmem_range kmlk_mrange(struct ultra_memory_map_entry* ent) {
	return (struct kmlk_pmem_range) {
		ent->physical_address,
		ent->size / KMLK_PAGE
	};
}

void* kmlk_set_memory(struct ultra_boot_context* ctx) {
	// TODO: These protections should probably change with mapping type.
	static const enum kmlk_mem_prot prot = KMLK_P_R | KMLK_P_W | KMLK_P_X;

	enum kml_result res;

	struct ultra_attribute_header* attr = ctx->attributes;
	struct ultra_kernel_info_attribute* kinfo = 0;
	struct ultra_memory_map_attribute* map = 0;

	void* mmctx = 0;

	for(kml_size_t i = 0; i < ctx->attribute_count; ++i) {
		if(attr->type == ULTRA_ATTRIBUTE_KERNEL_INFO) kinfo = (void*) attr;
		if(attr->type == ULTRA_ATTRIBUTE_MEMORY_MAP) map = (void*) attr;

		attr = ULTRA_NEXT_ATTRIBUTE(attr);
	}

	if(!kinfo || !map) {
		kml_dputs("invalid boot context\n");
		return 0;
	}

	kml_size_t rcount = ULTRA_MEMORY_MAP_ENTRY_COUNT(map->header);

	for(kml_size_t i = 0; i < rcount; ++i) {
		struct ultra_memory_map_entry* entry = &map->entries[i];

		if(entry->type != ULTRA_MEMORY_TYPE_FREE) continue;

		kmlk_palloc_append(kmlk_mrange(entry));
	}

	for(kml_size_t i = 0; i < rcount; ++i) {
		struct ultra_memory_map_entry* entry = &map->entries[i];

		kml_ptr_t vaddr = entry->physical_address;
		res = kmlk_mmap_range(&mmctx, kmlk_mrange(entry), vaddr, prot);
		if(res) goto mmdie;

		vaddr = KMLK_HIGHHALF + entry->physical_address;
		res = kmlk_mmap_range(&mmctx, kmlk_mrange(entry), vaddr, prot);
		if(res) goto mmdie;
	}

	struct kmlk_pmem_range krange = {
			kinfo->physical_base,
			kinfo->size / KMLK_PAGE
	};

	res = kmlk_mmap_range(&mmctx, krange, kinfo->virtual_base, prot);
	if(res) goto mmdie;

	return mmctx;

	mmdie: {
		// TODO: This should be recoverable.
		kml_presult("kmlk_mmap_range", res);
		return 0;
	}
}

// x86_64 `mmctx` is a pointer to the top level page entry.

static kml_size_t kmlk_pidx(kml_ptr_t vaddr, kml_u8_t lvl) {
	return (vaddr >> (12 + (9 * lvl))) & 0b111111111;
}

enum kml_result kmlk_mmap(
		void** mmctx, kmlk_paddr_t paddr, kml_ptr_t vaddr,
		enum kmlk_mem_prot prot) {

	if(vaddr & (KMLK_PAGE - 1)) return KML_E_ALIGN;
	if(paddr & (KMLK_PAGE - 1)) return KML_E_ALIGN;

	if(!*mmctx && !(*mmctx = kmlk_pcalloc())) return KML_E_OOM;

	struct kmlk_pt_entry* top = *mmctx;
	struct kmlk_pt_entry* ent = &top[kmlk_pidx(vaddr, 3)];

	for(kml_size_t i = 0; i < 3; ++i) {
		if(!(ent->present)) {
			void* p = kmlk_pcalloc();
			// TODO: This leaks unnecessarily allocated leaves.
			if(!p) return KML_E_OOM;

			ent->address = (kml_ptr_t) p >> 12;
			ent->present = KML_TRUE;
			ent->writeable = KML_TRUE;
		}

		kml_ptr_t addr = ent->address << 12;
		ent = &((struct kmlk_pt_entry*) addr)[kmlk_pidx(vaddr, 2 - i)];
	}

	ent->address = paddr >> 12;
	ent->present = KML_TRUE;
	ent->writeable = KML_TRUE;

	return KML_OK;
}

enum kml_result kmlk_mmap_range(
		void** mmctx, struct kmlk_pmem_range prange, kml_ptr_t vaddr,
		enum kmlk_mem_prot prot) {

	/*kml_dputf(
			"mapping $X pages from $X->$X\n",
			prange.count, prange.base, vaddr);*/

	for(kml_size_t i = 0; i < prange.count; ++i) {
		enum kml_result res;

		kml_size_t off = i * KMLK_PAGE;

		res = kmlk_mmap(mmctx, prange.base + off, vaddr + off, prot);
		if(res) return res; // TODO: This is leaky.
	}

	return KML_OK;
}

void kmlk_mmflush(void* mmctx) {
	union kmlk_cr3 cr3 = { 0 };

	cr3.table_address = (kml_ptr_t) mmctx >> 12;

	asm("movq %%rax, %%cr3" :: [value] "a" (cr3.cr3) : );
}
