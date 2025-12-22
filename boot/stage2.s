bits 32
org 0x9000

ELF_BASE equ 0x20000

protected_entry:
    cli
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000 ; temp stack

    call check_a20 ; checks a20
    call test_print ; prints zzboot

    call elf_loader

test_print:
    mov byte [0xB8000], 'Z'
    mov byte [0xB8002], 'Z'
    mov byte [0xB8004], 'B'
    mov byte [0xB8006], 'O'
    mov byte [0xB8008], 'O'
    mov byte [0xB800A], 'T'
    mov byte [0xB8001], 0x0F
    mov byte [0xB8003], 0x0F
    mov byte [0xB8005], 0x0F
    mov byte [0xB8007], 0x0F
    mov byte [0xB8009], 0x0F
    mov byte [0xB800B], 0x0F

    ret

check_a20:
    push ds
    push es
    
    xor ax, ax
    mov ds, ax
    mov es, ax

    mov word [0x2000], 0x1337
    mov word [0x102000], 0x3713

    cmp word [0x2000], 0x3713
    je .wompwomp

    pop es
    pop ds
    ret

.wompwomp:
    mov byte [0xB8200], 'N'
    mov byte [0xB8202], 'O'
    mov byte [0xB8204], ' '
    mov byte [0xB8206], 'A'
    mov byte [0xB8208], '2'
    mov byte [0xB820A], '0'
    cli
    hlt

elf_loader:
    mov esi, [ELF_BASE + 0x1C]
    add esi, ELF_BASE ; esi = first program header
    
    mov eax, [esi + 0x00]
    cmp eax, 1 ; PT_LOAD
    jne skip

    mov ebx, [esi + 0x04] ; p_offset
    mov edi, [esi + 0x0C] ; p_addr
    mov ecx, [esi + 0x10] ; p_filesz

    mov esi, ELF_BASE
    add esi, ebx

    cld
    rep movsb

    mov eax, [ELF_BASE + 0x18]
    jmp eax

skip:
    ret

halt:
    cli
    hlt

times 16384-($-$$) db 0
