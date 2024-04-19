// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KMLK_MEMORY_H
#define KMLK_MEMORY_H

#include <kml/common.h>

#define KMLK_PAGEDOWN(x) ((x) & ~(KMLK_PAGE - 1))
#define KMLK_PAGEUP(x) KMLK_PAGEDOWN((x) + KMLK_PAGE - 1)

typedef kml_ptr_t kmlk_paddr_t;

enum [[clang::flag_enum]] kmlk_mmflag {
	KMLK_PROT_NONE = 0,
	KMLK_PROT_READ = 1,
	KMLK_PROT_WRITE = 1 << 1,
	KMLK_PROT_EXEC = 1 << 2,

	// Mark entries as global (for kernel mappings).
	// Should only be set by arch specific code.
	KMLK_MM_GLOBAL = 1 << 3,

	// Useful smaller identifiers which aren't as descriptive.
	KMLK_P_0 = KMLK_PROT_NONE,
	KMLK_P_R = KMLK_PROT_READ,
	KMLK_P_W = KMLK_PROT_WRITE,
	KMLK_P_X = KMLK_PROT_EXEC
};

struct kmlk_pmem_range {
	kmlk_paddr_t base;
	kml_size_t count; // Count in pages.
};

// Add new range to the pmem allocator.
void kmlk_palloc_append(struct kmlk_pmem_range);

void* kmlk_palloc(void);
void* kmlk_pcalloc(void);
void kmlk_pfree(void*);

// First param opaque arch mman handle/top level page ptr for a given mapping
// Table.
enum kml_result kmlk_mmap(void**, kmlk_paddr_t, kml_ptr_t, enum kmlk_mmflag);
enum kml_result kmlk_mmap_range(
		void**, struct kmlk_pmem_range, kml_ptr_t, enum kmlk_mmflag);

// Copy all global (kernel) PT entries to a new mmctx.
enum kml_result kmlk_mmcopy_global(void**, void*);

void kmlk_mmflush(void*);

#endif
