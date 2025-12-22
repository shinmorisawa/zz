bits 32
global _start
extern kstart

_start:
    mov esp, 0x90000
    call kstart
.hang:
    cli
    hlt
    jmp .hang
