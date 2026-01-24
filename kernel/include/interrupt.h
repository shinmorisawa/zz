#pragma once

#include "types.h"

#define PIT_FREQ 1193182
#define PIT_DIVIDER (PIT_FREQ / CONFIG_PIT_TARGET_SPEED)

typedef struct __attribute__((__packed__)) IDTEntry {
    u16 offset_low;
    u16 segment;
    u8  ist;
    u8  type;
    u16 offset_mid;
    u32 offset_high;
    u32 reserved;
} IDTEntry;

typedef struct __attribute__((__packed__)) IDTPointer {
    u16 limit;
    u64 base;
} IDTPointer;

void idt_add_entry(int n, void (*handler)());
void idt_init(void);

__attribute__((no_caller_saved_registers))
void pic_eoi(void);
