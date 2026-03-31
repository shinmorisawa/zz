#include "types.h"
#include "paging.h"

PML4Entry* pml4 = (PML4Entry*)PML4_BASE;
PDPTEntry* pdpt = (PDPTEntry*)PDPT_BASE;
PDEntry* pd = (PDEntry*)PD_BASE;

void paging_install(void) {
    asm volatile (
        "mov %0, %%cr3"
        :
        : "r"(pml4)
        : "memory"
    );
}

void paging_init(void) {
    for (int i = 0; i < 512; i++) {
        pml4[i] = 0;
        pdpt[i] = 0;
        pd[i]   = 0;
    }

    pml4[0]   = (PDPT_BASE & 0x000FFFFFFFFFF000) | PTE_PRESENT | PTE_WRITABLE;
    pml4[511] = (PDPT_BASE & 0x000FFFFFFFFFF000) | PTE_PRESENT | PTE_WRITABLE;
    pdpt[0]   = (PD_BASE & 0x000FFFFFFFFFF000) | PTE_PRESENT | PTE_WRITABLE;
    pdpt[511] = (PD_BASE & 0x000FFFFFFFFFF000) | PTE_PRESENT | PTE_WRITABLE;

    for (int i = 0; i < 512; i++) {
        pd[i] = ((u64)i * 0x200000) | PTE_PRESENT | PTE_WRITABLE | PTE_PS;
    }

    static PDEntry pd1[512] __attribute__((aligned(4096)));
    for (int i = 0; i < 512; i++) {
        pd1[i] = ((u64)1 * 0x40000000ULL + (u64)i * 0x200000ULL) | PTE_PRESENT | PTE_WRITABLE | PTE_PS;
    }

    pdpt[1] = ((u64)&pd1 & 0x000FFFFFFFFFF000) | PTE_PRESENT | PTE_WRITABLE;

    static PDEntry pd2[512] __attribute__((aligned(4096)));
    for (int i = 0; i < 512; i++) {
        pd2[i] = ((u64)2 * 0x40000000ULL + (u64)i * 0x200000ULL) | PTE_PRESENT | PTE_WRITABLE | PTE_PS;
    }

    pdpt[2] = ((u64)&pd2 & 0x000FFFFFFFFFF000) | PTE_PRESENT | PTE_WRITABLE;

    static PDEntry pd3[512] __attribute__((aligned(4096)));
    for (int i = 0; i < 512; i++) {
        pd3[i] = ((u64)3 * 0x40000000ULL + (u64)i * 0x200000ULL) | PTE_PRESENT | PTE_WRITABLE | PTE_PS;
    }

    pdpt[3] = ((u64)&pd3 & 0x000FFFFFFFFFF000) | PTE_PRESENT | PTE_WRITABLE;

    paging_install();
}
