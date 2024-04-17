// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KMLK_MEMORY_H
#define KMLK_MEMORY_H

#include <kml/common.h>

#define KMLK_PAGEDOWN(x) ((x) & ~(KMLK_PAGE - 1))
#define KMLK_PAGEUP(x) KMLK_PAGEDOWN((x) + KMLK_PAGE - 1)

typedef kml_ptr_t kmlk_paddr_t;

struct kmlk_mem_range {
	kmlk_paddr_t base;
	kml_size_t count; // Count in pages.
};

// Add new range to the pmem allocator.
void kmlk_palloc_append(struct kmlk_mem_range*);

void* kmlk_palloc(void);
void kmlk_pfree(void*);

#endif
