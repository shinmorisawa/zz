#include "types.h"
#if CONFIG_UART
#include "uart.h"
#endif
#include "paging.h"
#include "interrupt.h"
#include "handlers.h"
#include "memory.h"
#include "utils.h"
#include "vm.h"

void halt(void) {
    asm volatile ("hlt" : : :);
}

extern volatile u64 ticks;

void kstart(void) {
    paging_init();
    #if CONFIG_UART
    uart_init();
    #endif
    idt_init();

    E820Entry* entries = (E820Entry*)0x504;
    u16 count = *(u16*)0x500;
    phys_alloc_init(entries, count);
    for (u16 i = 0; i < count; i++) {
        klog("idx:%x base:%x, length:%x, type:%x, acpi:%x\n", i, entries[i].base, entries[i].length, entries[i].type, entries[i].acpi);
    }

    klog("i like eating cereal because i'm serial!\n");

    for (int i = 0; i < 16; i++) {
        u8* addr = (u8*)(0x1000000 + (i * PAGE_SIZE));
        vm_map((u64)addr, (u64)phys_alloc(), PTE_WRITABLE);
        addr[1] = 42; 
        if (addr[1] == 42) {
            klog("[init] write test successful\n");
        } else {
            klog("[init] write test failed\n");
        }
    }

    u32* fb = (u32*)(u64)(*(u32*)0x6000);
    u16 pitch = *(u16*)0x6004;
    u16 width = *(u16*)0x6006;
    u16 height = *(u16*)0x6008;

    klog("fb addr:%x, pitch:%x, width:%d, height:%d\n", fb, pitch, width, height);

    for (u32 y = 0; y < height; y++) {
        u32* row = (u32*)(((u8*)fb) + y * pitch);
        for (u32 x = 0; x < width; x++) {
            row[x] = 0x007199EE;
        }
    }
}
