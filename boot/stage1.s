bits 16
section .stage1
global protected_init
extern long_trampoline

protected_init:
    cli ; this is like the third time just to be safe

    ; do real mode routines here (such as e820 or VESA)
    call e820_stuff
    call vga_mode_13

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_entry

e820_stuff:
    pusha
    mov di, 0x504           ; entries start at 0x504, count stored at 0x500
    xor ebx, ebx            ; ebx must be 0 to start
    xor bp, bp              ; entry count
    mov edx, 0x534D4150     ; "SMAP"

    ; first call
    mov eax, 0xE820
    mov [es:di + 20], dword 1   ; force valid ACPI 3.X entry
    mov ecx, 24
    int 0x15
    jc .failed              ; carry on first call = unsupported
    mov edx, 0x534D4150     ; restore in case BIOS trashed it
    cmp eax, edx            ; eax must be "SMAP" on success
    jne .failed
    test ebx, ebx           ; ebx = 0 means only one entry (useless)
    je .failed
    jmp .check
.loop:
    mov eax, 0xE820         ; trashed on every call, restore it
    mov edx, 0x534D4150     ; some BIOSes trash this too
    mov [es:di + 20], dword 1
    mov ecx, 24
    int 0x15
    jc .done                ; carry = end of list
.check:
    jcxz .next              ; skip 0 length response
    cmp cl, 20              ; did we get a 24 byte ACPI 3.X response?
    jbe .notext
    test byte [es:di + 20], 1   ; is the ignore bit clear?
    je .next
.notext:
    mov ecx, [es:di + 8]    ; lower 32 bits of region length
    or ecx, [es:di + 12]    ; OR with upper 32 bits to check for zero
    jz .next                ; skip zero length entries
    inc bp
    add di, 24
.next:
    test ebx, ebx           ; ebx = 0 means list complete
    jnz .loop
.done:
    mov [0x500], bp         ; store entry count at 0x500
    clc                     ; clear carry flag
    popa
    ret
.failed:
    stc                     ; set carry to signal failure
    popa
    ret

vga_mode_13:
    mov ax, 0x4F00
    mov di, 0x7000
    int 0x10
    cmp ax, 0x004F
    jne .failed

    mov ax, 0x4F01
    mov cx, 0x143
    mov di, 0x7200
    int 0x10
    cmp ax, 0x004F
    jne .failed
    
    mov ax, 0x4F02
    mov bx, 0x143
    or  bx, 0x4000
    int 0x10
    cmp ax, 0x004F
    jne .failed

    mov eax, [0x7200 + 0x28]
    mov [0x6000], eax
    mov ax, [0x7200 + 0x10]
    mov [0x6004], ax
    mov ax, [0x7200 + 0x12]
    mov [0x6006], ax
    mov ax, [0x7200 + 0x14]
    mov [0x6008], ax

    ret
.failed:
    ret

bits 32
section .stage1

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

    jmp start_long

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

align 8
gdt_start:
    dq 0x0000000000000000 ; null
    dq 0x00CF9A000000FFFF ; code
    dq 0x00CF92000000FFFF ; data
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

align 4096
pml4:
    dq pdpt + 0x003
    times 511 dq 0

align 4096
pdpt:
    dq pd + 0x003
    times 511 dq 0

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
