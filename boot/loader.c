#include <stdint.h>

#define ELF_BASE 0x20000

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef struct {
    u8  e_ident[16];
    u16 e_type;
    u16 e_machine;
    u32 e_version;
    u64 e_entry;
    u64 e_phoff;
    u64 e_shoff;
    u32 e_flags;
    u16 e_ehsize;
    u16 e_phentsize;
    u16 e_phnum;
    u16 e_shentsize;
    u16 e_shnum;
    u16 e_shstrndx;
} Elf64_Ehdr;

typedef struct {
    u32 p_type;
    u32 p_flags;
    u64 p_offset;
    u64 p_vaddr;
    u64 p_paddr;
    u64 p_filesz;
    u64 p_memsz;
    u64 p_align;
} Elf64_Phdr;

__attribute__((section(".stage2")))
void halt(void) {
    asm volatile ("cli\nhlt" : : :);
}

__attribute__((section(".stage2")))
void memset(u8* ptr, u8 value, u64 amount) {
    for (u64 i = 0; i < amount; i++) {
        ptr[i] = value;
    }
}

__attribute__((section(".stage2")))
void memcpy(void* dst, const void* src, u64 n) {
    u8* d = (u8*)dst;
    const u8* s = (const u8*)src;

    for (u64 i = 0; i < n; i++) {
        d[i] = s[i];
    }
}

__attribute__((section(".stage2")))
void load_elf(void) {
    Elf64_Ehdr* header = (Elf64_Ehdr*)ELF_BASE;
    if (header->e_ident[0] != 0x7F || header->e_ident[1] != 'E' || header->e_ident[2] != 'L' || header->e_ident[3] != 'F') {
        halt();
    }

    if (header->e_ident[4] != 2) {
        halt();
    }

    Elf64_Phdr* program_header;
    for (int i = 0; i < header->e_phnum; i++) {
        program_header = (Elf64_Phdr*)((u8*)header + header->e_phoff + i * header->e_phentsize);
        if (program_header->p_type != 1) continue;

        memcpy((void*)program_header->p_paddr, (void*)(ELF_BASE + program_header->p_offset), program_header->p_filesz);

        if (program_header->p_memsz > program_header->p_filesz) {
            memset((u8*)(program_header->p_paddr + program_header->p_filesz), 0, program_header->p_memsz - program_header->p_filesz);
        }
    }

    ((void(*)(void))header->e_entry)();
}
