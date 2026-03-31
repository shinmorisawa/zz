#pragma once

#include "config.h"
#include "types.h"

#define PAGE_SIZE 0x1000
#define MAX_PAGES CONFIG_MAX_PAGES
#define BITMAP_BASE 0x500000

typedef struct E820Entry {
    u64 base;
    u64 length;
    u32 type;
    u32 acpi;
} __attribute__((packed)) E820Entry;

void phys_alloc_init(const E820Entry* entries, const u16 count);
void* phys_alloc(void);
void phys_free(void* addr);
