// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

#include <kmlk/memory.h>

#include <kmlk/arch/x86_64/tables.h>

#include <ultra_protocol.h>

static struct kmlk_pmem_range kmlk_pmrange(struct ultra_memory_map_entry* ent) {
	return (struct kmlk_pmem_range) {
		ent->physical_address,
		ent->size / KMLK_PAGE
	};
}

static struct kmlk_pmem_range kmlk_vmrange(struct ultra_memory_map_entry* ent) {
	return (struct kmlk_pmem_range) {
			KMLK_HIGHHALF | ent->physical_address,
			ent->size / KMLK_PAGE
	};
}

void* kmlk_set_memory(struct ultra_boot_context* ctx) {
	enum kml_result res;

	struct ultra_attribute_header* attr = ctx->attributes;
	struct ultra_kernel_info_attribute* kinfo = 0;
	struct ultra_memory_map_attribute* map = 0;

	void* mmctx = 0;

	for(kml_size_t i = 0; (!kinfo && !map) || i < ctx->attribute_count; ++i) {
		if(attr->type == ULTRA_ATTRIBUTE_KERNEL_INFO) kinfo = (void*) attr;
		if(attr->type == ULTRA_ATTRIBUTE_MEMORY_MAP) map = (void*) attr;

		attr = ULTRA_NEXT_ATTRIBUTE(attr);
	}

	if(!kinfo || !map) {
		KML_DLOG("invalid boot context");
		return 0;
	}

	kml_size_t rcount = ULTRA_MEMORY_MAP_ENTRY_COUNT(map->header);

	for(kml_size_t i = 0; i < rcount; ++i) {
		struct ultra_memory_map_entry* entry = &map->entries[i];

		if(entry->type != ULTRA_MEMORY_TYPE_FREE) continue;

		kmlk_palloc_append(kmlk_vmrange(entry));
	}

	for(kml_size_t i = 0; i < rcount; ++i) {
		struct ultra_memory_map_entry* entry = &map->entries[i];

		enum kmlk_mmflag prot = KMLK_MM_GLOBAL | KMLK_P_R;

		if(entry->type == ULTRA_MEMORY_TYPE_FREE ||
			entry->type == ULTRA_MEMORY_TYPE_RECLAIMABLE ||
			entry->type == ULTRA_MEMORY_TYPE_LOADER_RECLAIMABLE ||
			entry->type == ULTRA_MEMORY_TYPE_KERNEL_STACK) {

			prot |= KMLK_P_W;
		}

		// For some reason we if kernel stack is marked noexec here.
		if(entry->type == ULTRA_MEMORY_TYPE_KERNEL_STACK ||
			entry->type == ULTRA_MEMORY_TYPE_KERNEL_BINARY) {

			prot |= KMLK_P_X;
		}

		kml_ptr_t vaddr = KMLK_HIGHHALF + entry->physical_address;
		res = kmlk_mmap_range(&mmctx, kmlk_pmrange(entry), vaddr, prot);
		if(res) goto mmdie;
	}

	struct kmlk_pmem_range krange = {
			kinfo->physical_base,
			kinfo->size / KMLK_PAGE
	};

	enum kmlk_mmflag prot = KMLK_MM_GLOBAL | KMLK_P_R | KMLK_P_X;
	res = kmlk_mmap_range(&mmctx, krange, kinfo->virtual_base, prot);
	if(res) goto mmdie;

	return mmctx;

	mmdie: {
		// TODO: This should be recoverable.
		kml_presult("kmlk_mmap_range", res);
		return 0;
	}
}

static kml_bool_t kmlk_mem_done = KML_FALSE;

// TODO: This has a load of code duplication -- we could hack our way around
//		 This by just having the map become global in the first call but let's
//		 Wait until the rest of the boot infrastructure is set up before we
//		 Start abstracting Hyper.
void kmlk_done_arch(struct ultra_boot_context* ctx) {
	// TODO: Need to copy out reclaimable entries -- can't just iterate like
	//		 This because it will clobber itself when initializing the `palloc`
	//		 Ranges in `kmlk_palloc_append`.

	(void) ctx;

	/*
	struct ultra_memory_map_attribute* map = 0;

	struct ultra_attribute_header* attr = ctx->attributes;
	for(kml_size_t i = 0; i < ctx->attribute_count; ++i) {
		if(attr->type == ULTRA_ATTRIBUTE_MEMORY_MAP) {
			map = (void*) attr;
			break;
		}

		attr = ULTRA_NEXT_ATTRIBUTE(attr);
	}

	kml_size_t rcount = ULTRA_MEMORY_MAP_ENTRY_COUNT(map->header);

	for(kml_size_t i = 0; i < rcount; ++i) {
		struct ultra_memory_map_entry* entry = &map->entries[i];

		if(entry->type != ULTRA_MEMORY_TYPE_RECLAIMABLE ||
			entry->type != ULTRA_MEMORY_TYPE_LOADER_RECLAIMABLE) {

			continue;
		}

		kmlk_palloc_append(kmlk_vmrange(entry));
	}
	 */

	kmlk_mem_done = KML_TRUE;
}

// x86_64 `mmctx` is a pointer to the top level page entry.

static kml_size_t kmlk_pidx(kml_ptr_t vaddr, kml_u8_t lvl) {
	return (vaddr >> (12 + (9 * lvl))) & 0b111111111;
}

enum kml_result kmlk_mmap(
		void** mmctx, kmlk_paddr_t paddr, kml_ptr_t vaddr,
		enum kmlk_mmflag fl) {

	if(vaddr & (KMLK_PAGE - 1)) return KML_E_ALIGN;
	if(paddr & (KMLK_PAGE - 1)) return KML_E_ALIGN;

	if(!*mmctx && !(*mmctx = kmlk_pcalloc())) return KML_E_OOM;

	struct kmlk_pt_entry* top = *mmctx;
	struct kmlk_pt_entry* ent = &top[kmlk_pidx(vaddr, 3)];

	for(kml_size_t i = 0; i < 3; ++i) {
		if(!(ent->present)) {
			// TODO: Add option to preallocate entire top level to ensure all
			//		 Future `kmmap` are "safe".
			// Once arch boot is done it's no longer safe to modify the top
			// Level table -- as processes may have inherited the old table.
			if(kmlk_mem_done && i == 0 && (fl & KMLK_MM_GLOBAL)) {
				return KML_E_LATE;
			}

			void* p = kmlk_pcalloc();
			// TODO: This leaks unnecessarily allocated leaves.
			if(!p) return KML_E_OOM;

			ent->address = ((kml_ptr_t) p & ~KMLK_HIGHHALF) >> 12;
			ent->present = KML_TRUE;
			ent->writeable = KML_TRUE;
		}

		kml_ptr_t addr = KMLK_HIGHHALF | (ent->address << 12);
		ent = &((struct kmlk_pt_entry*) addr)[kmlk_pidx(vaddr, 2 - i)];
	}

	ent->address = paddr >> 12;
	ent->present = KML_TRUE;
	ent->writeable = !!(fl & KMLK_PROT_WRITE);
	ent->no_execute = !(fl & KMLK_PROT_EXEC);
	ent->lvl1_global = !!(fl & KMLK_MM_GLOBAL);

	return KML_OK;
}

enum kml_result kmlk_mmap_range(
		void** mmctx, struct kmlk_pmem_range prange, kml_ptr_t vaddr,
		enum kmlk_mmflag prot) {

	KML_DLOG("mapping $X pages from $X->$X", prange.count, prange.base, vaddr);

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

	cr3.table_address = ((kml_ptr_t) mmctx & ~KMLK_HIGHHALF) >> 12;

	asm("movq %%rax, %%cr3" :: [value] "a" (cr3.cr3) : );
}
