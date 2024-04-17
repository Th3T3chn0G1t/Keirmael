// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

#ifndef KMLK_KERNEL_H
#define KMLK_KERNEL_H

[[noreturn]] void kmlk_start(void);

[[noreturn]] void kmlk_hang(void);
[[noreturn]] void kmlk_panic(const char*);

#endif
