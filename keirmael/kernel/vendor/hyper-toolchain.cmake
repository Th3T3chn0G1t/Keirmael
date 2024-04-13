# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) 2024 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

# TODO: We're just not gonna bother with UEFI targets right now. Fix this in
#       The future!

# Do this because cmake attempts to link against -lc and crt0
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_LINK_EXECUTABLE
        "<CMAKE_LINKER> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> \
        -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --target=i686-none-none")

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ${HYPER_ARCH})
set(CMAKE_SYSROOT "")
