#include "types.h"
#include "memory.h"
#include "config.h"
#include "utils.h"

u8* bitmap = (u8*)BITMAP_BASE;

u64 reserved_end = 0x600000;

void phys_alloc_init(const E820Entry* entries, const u16 count) {
    memset(bitmap, 0xFF, MAX_PAGES / 8);

    for (u16 i = 0; i < count; i++) {
        if (entries[i].type != 1) continue;

        u64 start = entries[i].base / PAGE_SIZE;
        u64 end   = (entries[i].base + entries[i].length) / PAGE_SIZE;

        for (u64 j = start; j < end && j < MAX_PAGES; j++) {
            bitmap[j / 8] &= ~(1 << (j % 8));
        }
    }

    for (u64 i = 0; i < (reserved_end / PAGE_SIZE); i++) {
        bitmap[i / 8] |= (1 << (i % 8));
    }
}

void* phys_alloc(void) {
    for (u64 counter = 1; counter < MAX_PAGES; counter++) {
        u64 byte_index = counter / 8;
        u64 bit_index = counter % 8;

        if (!((bitmap[byte_index] >> bit_index) & 1)) {
            void* addr = (void*)((byte_index * 8 + bit_index) * PAGE_SIZE);
            klog("[m] phys_alloc, idx:%x, addr:%x\n", counter, (u64)addr);
            bitmap[byte_index] |= (1 << bit_index);
            memset(addr, 0, PAGE_SIZE);
            return addr;
        }
    }

    klog("[m] phys_alloc failed, OOM commencing\n");
    kpanic("[m] memory failed");
    return NULL;
}

void phys_free(void* addr) {
    u64 page = (u64)addr / PAGE_SIZE;
    u64 byte_index = page / 8;
    u64 bit_index = page % 8;
    bitmap[byte_index] &= ~(1 << bit_index);
}
