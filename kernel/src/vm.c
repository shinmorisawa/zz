#include "types.h"
#include "memory.h"
#include "config.h"
#include "utils.h"
#include "paging.h"

extern PML4Entry* pml4;
extern PDPTEntry* pdpt;
extern PDEntry* pd;

void vm_map(u64 virt, u64 phys, u64 flags) {
    u64 pml4_idx = (virt >> 39) & 0x1FF;
    u64 pdpt_idx = (virt >> 30) & 0x1FF;
    u64 pd_idx   = (virt >> 21) & 0x1FF;
    u64 pt_idx   = (virt >> 12) & 0x1FF;

    if (!(pml4[pml4_idx] & PTE_PRESENT)) {
        u64 new_pdpt = (u64)phys_alloc();
        memset((void*)new_pdpt, 0, 4096);
        pml4[pml4_idx] = (new_pdpt & 0x000FFFFFFFFFF000) | PTE_PRESENT | PTE_WRITABLE;
    }
    PDPTEntry* pdpt = (PDPTEntry*)(pml4[pml4_idx] & 0x000FFFFFFFFFF000);

    if (!(pdpt[pdpt_idx] & PTE_PRESENT)) {
        u64 new_pd = (u64)phys_alloc();
        memset((void*)new_pd, 0, 4096);
        pdpt[pdpt_idx] = (new_pd & 0x000FFFFFFFFFF000) | PTE_PRESENT | PTE_WRITABLE;
    }
    PDEntry* pd = (PDEntry*)(pdpt[pdpt_idx] & 0x000FFFFFFFFFF000);

    if (!(pd[pd_idx] & PTE_PRESENT)) {
        u64 new_pt = (u64)phys_alloc();
        memset((void*)new_pt, 0, 4096);
        pd[pd_idx] = (new_pt & 0x000FFFFFFFFFF000) | PTE_PRESENT | PTE_WRITABLE;
    }
    PTEntry* pt = (PTEntry*)(pd[pd_idx] & 0x000FFFFFFFFFF000);

    pt[pt_idx] = (phys & 0x000FFFFFFFFFF000) | flags | PTE_PRESENT;

    asm volatile ("invlpg (%0)" :: "r"(virt) : "memory");

    klog("[vm] new vm_map %x -> %x\n", virt, phys);
}
