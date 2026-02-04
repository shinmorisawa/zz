bits 64
section .text

extern interrupt_handler

isr_stub:
    cld

    push rax
    push r11
    push r10
    push r9
    push r8
    push rdi
    push rsi
    push rdx
    push rcx

    mov rdi, rsp
    call interrupt_handler

    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop r8
    pop r9
    pop r10
    pop r11
    pop rax

    add rsp, 8

    iretq

%macro ISR_NO_ERROR 1
global isr%1
isr%1:
    push 0
    push %1
    jmp isr_stub
%endmacro

%macro ISR_ERROR 1
global isr%1
isr%1:
    push %1
    jmp isr_stub
%endmacro

ISR_NO_ERROR 0
ISR_NO_ERROR 1

ISR_ERROR 14

ISR_NO_ERROR 32
