section .mbr
bits 16
extern protected_init

mbr_start:
    cli
    mov sp, 0x4000
    xor ax, ax
    mov es, ax

    mov [0x2000], dl

    ;call vesa_init ; init vesa!!!
    call enable_a20 ; enables a20, aka high mem
    call load_stage1 ; stage1 because that's what's next
    call load_kernel ; load the kernel elf file
    jmp protected_init ; go into protected mode

enable_a20:
    in al, 0x92
    or al, 0x02
    out 0x92, al
    ret

load_stage1:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov dl, [0x2000]
    mov si, dap
    mov ah, 0x42

    int 0x13
    jc error

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov dl, 0x80
    mov si, dap_stage2
    mov ah, 0x42

    int 0x13
    jc error

    ret

load_kernel:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov dl, 0x80
    mov si, dap_elf1
    mov ah, 0x42

    int 0x13
    jc error

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov dl, 0x80
    mov si, dap_elf2
    mov ah, 0x42

    int 0x13
    jc error

    ret

;vesa_init:
;    mov ax, 0x4F00
;    int 0x10
;
;    mov ax, 0x1000
;    mov es, ax
;    mov cx, 0x101
;    mov di, 0
;    mov ax, 0x4F01 ; get mode info
;    int 0x10
;
;    cmp ax, 0x004F
;    jne .failed
;
;    mov ax, 0x4F02 ; set mode
;    mov bx, 0x101 ; getting desparate
;    or  bx, 0x4000 ; linear
;    int 0x10
;
;    cmp ax, 0x004F
;    jne .failed
;
;.failed:
;    ret

error:
    cli
    hlt

align 16
dap:
    db 16
    db 0
    dw 32
    dw 0x8000
    dw 0x0000
    dq 2

align 16
dap_stage2:
    db 16
    db 0
    dw 1
    dw 0xC000
    dw 0x0000
    dq 34

align 16
dap_elf1:
    db 16
    db 0
    dw 128
    dw 0x0000
    dw 0x2000
    dq 35

align 16
dap_elf2:
    db 16
    db 0
    dw 128
    dw 0x0000
    dw 0x3000
    dq 163
