#pragma once

#include "types.h"

typedef struct __attribute__((__packed__)) IDTEntry {
    u16 offset_low;
    u16 segment;
    u8  ist;
    u8  type;
    u16 offset_mid;
    u32 offset_high;
    u32 reserved;
} IDTEntry;
