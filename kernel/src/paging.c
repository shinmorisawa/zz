#include "types.h"
#include "paging.h"

static PML4Entry* pml4 = (PML4Entry*)PML4_BASE;
static PDPTEntry* pdpt = (PDPTEntry*)PDPT_BASE;
static PDEntry* pd = (PDEntry*)PD_BASE;

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

    for (int i = 0; i < 64; i++) {
        pd[i] = ((u64)i * 0x200000) | PTE_PRESENT | PTE_WRITABLE | PTE_PS;
    }

    paging_install();
}
