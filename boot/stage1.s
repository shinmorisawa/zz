bits 16
org 0x8000

stage1_start:
    cli
    xor ax, ax
    mov es, ax

    mmap_ent equ 0x5000            ; the number of entries will be stored at 0x5000

    call enable_a20 ; enables a20, aka high mem
    call do_e820
    call load_stage2 ; stage2 because that's what's next
    call load_kernel ; load the kernel elf file
    jmp protected_init ; go into protected mode

enable_a20:
    in al, 0x92
    or al, 0x02
    out 0x92, al
    ret

do_e820:
    mov di, 0x5004          ; Set di to 0x5004. Otherwise this code will get stuck in `int 0x15` after some entries are fetched 
	xor ebx, ebx		; ebx must be 0 to start
	xor bp, bp		; keep an entry count in bp
	mov edx, 0x0534D4150	; Place "SMAP" into edx
	mov eax, 0xe820
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes
	int 0x15
	jc short .failed	; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150	; Some BIOSes apparently trash this register?
	cmp eax, edx		; on success, eax must have been reset to "SMAP"
	jne short .failed
	test ebx, ebx		; ebx = 0 implies list is only 1 entry long (worthless)
	je short .failed
	jmp short .jmpin
.e820lp:
	mov eax, 0xe820		; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes again
	int 0x15
	jc short .e820f		; carry set means "end of list already reached"
	mov edx, 0x0534D4150	; repair potentially trashed register
.jmpin:
	jcxz .skipent		; skip any 0 length entries
	cmp cl, 20		; got a 24 byte ACPI 3.X response?
	jbe short .notext
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je short .skipent
.notext:
	mov ecx, [es:di + 8]	; get lower uint32_t of memory region length
	or ecx, [es:di + 12]	; "or" it with upper uint32_t to test for zero
	jz .skipent		; if length uint64_t is 0, skip entry
	inc bp			; got a good entry: ++count, move to next storage spot
	add di, 24
.skipent:
	test ebx, ebx		; if ebx resets to 0, list is complete
	jne short .e820lp
.e820f:
	mov [es:mmap_ent], bp	; store the entry count
	clc			; there is "jc" on end of list to this point, so the carry must be cleared
	ret
.failed:
	stc			; "function unsupported" error exit
	ret

load_stage2:
    mov dl, 0x80
    mov si, dap
    mov ah, 0x42

    int 0x13
    jc error

    ret

load_kernel:
    mov dl, 0x80
    mov si, dap_elf
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

    jmp 0x08:0x9000

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
    dw 0x9000
    dw 0x0000
    dq 3

align 16
dap_elf:
    db 16
    db 0
    dw 512
    dw 0x0000
    dw 0x2000
    dq 35

times 1024-($-$$) db 0
