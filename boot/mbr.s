bits 16
org 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov sp, 0x4000
    
    call load_stage1 ; stage1 because i might want more

    jmp 0x0000:0x8000

dap:
    db 16
    db 0
    dw 1
    dw 0x8000
    dw 0x0000
    dq 1

load_stage1:
    mov dl, 0x80
    mov si, dap
    mov ah, 0x42

    int 0x13
    jc error

    ret

error:
    cli
    hlt

times 510-($-$$) db 0
dw 0xAA55
