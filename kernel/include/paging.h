#pragma once

#include "types.h"

#define PML4_BASE 0x400000
#define PDPT_BASE 0x401000
#define PD_BASE   0x402000

typedef u64 PML4Entry;
typedef u64 PDPTEntry;
typedef u64 PDEntry;

/* paging flags, thanks chatgpt */
#define PTE_PRESENT   (1ULL << 0)
#define PTE_WRITABLE  (1ULL << 1)
#define PTE_USER      (1ULL << 2)
#define PTE_PWT       (1ULL << 3)
#define PTE_PCD       (1ULL << 4)
#define PTE_ACCESSED  (1ULL << 5)
#define PTE_DIRTY     (1ULL << 6)
#define PTE_PS        (1ULL << 7)
#define PTE_GLOBAL    (1ULL << 8)
#define PTE_NX        (1ULL << 63)

void paging_init(void);
