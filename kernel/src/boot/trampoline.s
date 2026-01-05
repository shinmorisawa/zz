bits 64
global _start
extern kstart

_start:
    mov rsp, 0x90000
    and rsp, -16
    sub rsp, 8
    call kstart
.hang:
    cli
    hlt
    jmp .hang
