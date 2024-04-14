# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# Included by top level makefile.

# Sets host targets and tools.

ifdef OS
	WINDOWS = 1

	RM = del
	RMDIR = rmdir /s /q
	SEP = \\
else
	ifeq ($(shell uname -s),Darwin)
		APPLE = 1
	endif

	RM = rm
	RMDIR = rm -rf
	SEP = /
endif

ifdef WINDOWS
	LIB =
	OBJ = .obj
	EXE = .exe
	A = .lib
else
	LIB = lib
	OBJ = .o
# This isn't strictly neccesary but it makes discerning our rules a lot easier.
	EXE = .out
	A = .a
endif

%$(OBJ): HCFLAGS += $(SET_HCFLAGS)
%$(OBJ): CFLAGS += $(SET_CFLAGS)
%$(OBJ): %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(HCFLAGS)

%$(EXE):
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)
