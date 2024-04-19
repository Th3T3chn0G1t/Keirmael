// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KMLK_X86_64_START_H
#define KMLK_X86_64_START_H

struct ultra_boot_context;

// Set up default `palloc` and mappings.
void* kmlk_set_memory(struct ultra_boot_context*);

// Reclaims loader/firmware pages and marks arch boot as finished.
void kmlk_done_arch(struct ultra_boot_context*);

#endif
