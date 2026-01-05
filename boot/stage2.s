bits 32
org 0x9000

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

    call start_long
    jmp 0x08:long_trampoline

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
    jmp halt

start_long:
    lgdt [gdt64_descriptor]

    mov eax, pml4
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    jmp 0x08:long_trampoline

halt:
    cli
    hlt

align 8
gdt64:
    dq 0x0000000000000000 ; null
    dq 0x00209A000000FFFF ; code
    dq 0x000092000000FFFF ; data
gdt64_end:

gdt64_descriptor:
    dw gdt64_end - gdt64 - 1
    dq gdt64

align 4096
pml4:
    dq pdpt + 0x003

align 4096
pdpt:
    dq pd + 0x003

align 4096
pd:
    dq 0x00000000 + 0x083
    dq 0x00200000 + 0x083
    dq 0x00400000 + 0x083
    dq 0x00600000 + 0x083
    dq 0x00800000 + 0x083
    dq 0x00A00000 + 0x083
    dq 0x00C00000 + 0x083
    dq 0x00E00000 + 0x083
    times 504 dq 0

bits 64
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

    ; --- ELF header pointer ---
    mov rbx, ELF_BASE          ; base of ELF in memory
    mov r8, [rbx + 0x20]       ; e_phoff
    add r8, rbx                ; pointer to program header table
    movzx rcx, word [rbx + 0x38]  ; e_phnum
    xor rdx, rdx               ; index = 0

load_segments_loop:
    cmp rdx, rcx
    jae jump_entry              ; done

    mov rax, rdx
    shl rax, 5                  ; rdx*32
    mov r9, rdx
    shl r9, 3                   ; rdx*8
    add rax, r9                 ; rdx*40 = sizeof(Elf64_Phdr)
    add rax, r8                 ; address of program header

    ; check segment type
    mov esi, [rax + 0x00]
    cmp esi, 1
    jne skip_segment

    ; copy filesz
    mov rsi, [rax + 0x08]      ; p_offset
    add rsi, rbx                ; source in memory
    mov rdi, [rax + 0x10]      ; p_vaddr (identity = physical)
    mov r9, [rax + 0x20]       ; p_filesz
    test r9, r9
    jz zero_bss
copy_filesz:
    mov al, [rsi]
    mov [rdi], al
    inc rsi
    inc rdi
    dec r9
    jnz copy_filesz

zero_bss:
    mov r9, [rax + 0x28]       ; p_memsz
    sub r9, [rax + 0x20]       ; memsz - filesz
    test r9, r9
    jz next_segment
    xor al, al
zero_loop:
    mov [rdi], al
    inc rdi
    dec r9
    jnz zero_loop

next_segment:
    inc rdx
    jmp load_segments_loop

skip_segment:
    inc rdx
    jmp load_segments_loop

jump_entry:
    mov rax, [rbx + 0x18]      ; e_entry
    jmp rax

times 16896-($-$$) db 0
