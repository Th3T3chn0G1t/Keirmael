// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KMLK_X86_64_CPU_H
#define KMLK_X86_64_CPU_H

#include <kml/common.h>

// Static kernel base as defined in `kernel.ld`
#define KMLK_BASE ((kml_ptr_t) 0xFFFFFFFF80000000UL)

typedef kml_u64_t kmlk_register_t;

#endif
