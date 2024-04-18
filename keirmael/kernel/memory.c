// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>
#include <kml/bit.h>

#include <kmlk/memory.h>

struct [[gnu::packed]] kmlk_palloc_hdr {
	kml_size_t count;
	struct kmlk_palloc_hdr* next;
	kml_u8_t bitset[];
};

struct kmlk_palloc_hdr* kmlk_palloc_list = 0;

void kmlk_palloc_append(struct kmlk_pmem_range range) {
	struct kmlk_palloc_hdr* hdr = (void*) range.base;

	hdr->count = range.count;

	kml_size_t setcount = KML_ROUNDUP(hdr->count, 8) / 8;
	kml_size_t hdr_size = setcount + sizeof(struct kmlk_palloc_hdr);
	kml_size_t hdr_pages = KMLK_PAGEUP(hdr_size) / KMLK_PAGE;

	kml_memset(hdr->bitset, 0, setcount);

	hdr->next = kmlk_palloc_list;
	kmlk_palloc_list = hdr;

	// Mark the header pages as used to simplify calculating usage.
	for(kml_size_t i = 0; i < hdr_pages; ++i) {
		kml_bitset_set(hdr->bitset, i);
	}
}

void* kmlk_palloc(void) {
	struct kmlk_palloc_hdr* head = kmlk_palloc_list;

	do {
		for(kml_size_t i = 0; i < head->count; ++i) {
			if(!kml_bitset_get(head->bitset, i)) {
				kml_bitset_set(head->bitset, i);
				return ((kml_u8_t*) head) + (i * KMLK_PAGE);
			}
		}
	} while((head = head->next));

	return 0;
}

void* kmlk_pcalloc(void) {
	return kml_memset(kmlk_palloc(), 0, KMLK_PAGE);
}

void kmlk_pfree(void* p) {
	struct kmlk_palloc_hdr* head = kmlk_palloc_list;
	kml_u8_t* headc = (kml_u8_t*) head;
	kml_u8_t* pc = (kml_u8_t*) p;

	do {
		if(pc > headc && pc < headc + (head->count * KMLK_PAGE)) {
			kml_bitset_unset(head->bitset, (pc - headc) / KMLK_PAGE);
			return;
		}
	} while((head = head->next));

	kml_dputs("kmlk_pfree: address ");
	kml_dputx((kml_ptr_t) p);
	kml_dputs(" out of range\n");
}
