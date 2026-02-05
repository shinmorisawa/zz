bits 16
section .mbr
extern protected_entry

mbr_start:
    cli
    mov sp, 0x4000
    xor ax, ax
    mov es, ax

    mov [0x2000], dl

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

protected_init:
    cli ; this is like the third time just to be safe
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_entry

error:
    cli
    hlt

align 8
gdt_start:
    dq 0x0000000000000000 ; null
    dq 0x00CF9A000000FFFF ; code
    dq 0x00CF92000000FFFF ; data
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

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
