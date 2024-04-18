// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#include <kml/io.h>

#include <kmlk/kernel.h>
#include <kmlk/memory.h>

#include <kmlk/arch/x86_64/tables.h>
#include <kmlk/arch/x86_64/interrupt.h>

#include <ultra_protocol.h>

// TODO: Find a better place to put this.
[[noreturn]] void kmlk_hang(void) {
	asm("cli");
	while(1) asm("hlt");
}

void _start(struct ultra_boot_context* context, unsigned int magic) {
	asm("cli");

	// Random arch guff (gdt, idt+pic, tss etc.)
	kmlk_set_arch_tables();
	kmlk_set_interrupt();

	asm("int3");

	// Mem. map

	struct ultra_attribute_header* attr = context->attributes;
	for(kml_size_t i = 0; i < context->attribute_count; ++i) {
		kml_u32_t type = attr->type;

		switch(type) {
			// case ULTRA_ATTRIBUTE_PLATFORM_INFO:
			// case ULTRA_ATTRIBUTE_KERNEL_INFO:

			case ULTRA_ATTRIBUTE_MEMORY_MAP: {
				struct ultra_memory_map_attribute* map = (void*) attr;
				kml_size_t count = ULTRA_MEMORY_MAP_ENTRY_COUNT(*attr);

				for(kml_size_t j = 0; j < count; ++j) {
					struct ultra_memory_map_entry* entry = &map->entries[j];

					switch(entry->type) {
						default: continue;

						case ULTRA_MEMORY_TYPE_FREE: {
							kmlk_palloc_append((struct kmlk_mem_range) {
									entry->physical_address,
									entry->size / KMLK_PAGE
							});
							break;
						}
						// case ULTRA_MEMORY_TYPE_KERNEL_STACK:
						// case ULTRA_MEMORY_TYPE_RECLAIMABLE:
						// case ULTRA_MEMORY_TYPE_LOADER_RECLAIMABLE:
						// case ULTRA_MEMORY_TYPE_KERNEL_BINARY:
					}
				}

				break;
			}

			// case ULTRA_ATTRIBUTE_MODULE_INFO:
			// case ULTRA_ATTRIBUTE_COMMAND_LINE:
			// case ULTRA_ATTRIBUTE_FRAMEBUFFER_INFO:

			default: {
				kml_dputs("Unknown boot attribute: ");
				kml_dputx(type);
				kml_dputc('\n');
			}
		}

		attr = ULTRA_NEXT_ATTRIBUTE(attr);
	}

	// TODO: Append reclaimable pages here before handoff.

	kmlk_start();
}
