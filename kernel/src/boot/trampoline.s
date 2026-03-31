bits 64
global _start
extern kstart
extern kernel_stack_top

_start:
    cli ; for now, that is

    mov rsp, kernel_stack_top
    and rsp, -16
    sub rsp, 8
    mov rdi, 0x4F0
    call kstart

.hang:
    cli
    hlt
    jmp .hang
