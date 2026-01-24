bits 64
section .stage2
global long_trampoline
extern load_elf
ELF_BASE equ 0x20000   ; ELF loaded here

long_trampoline:
    cld ; why i need this? idk
    ; --- stack setup ---
    mov rsp, 0x90000
    xor rbp, rbp

    ; --- VGA test ---
    mov rdi, 0xB8000
    mov ax, 0x0F41      ; 'A' white on black
    mov [rdi], ax

    call load_elf


; times 16896-($-$$) db 0
