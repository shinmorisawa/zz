bits 32
global _start

section .text
_start:
    mov byte [0xB8000], 't'
    mov byte [0xB8002], 'i'
    mov byte [0xB8004], 'n'
    mov byte [0xB8006], 'y'
    mov byte [0xB8008], '3'
    mov byte [0xB800A], '2'
    mov byte [0xB800C], '.'
    mov byte [0xB800E], 'e'
    mov byte [0xB8010], 'l'
    mov byte [0xB8012], 'f'

    mov byte [0xB8001], 0x0F
    mov byte [0xB8003], 0x0F
    mov byte [0xB8005], 0x0F
    mov byte [0xB8007], 0x0F
    mov byte [0xB8009], 0x0F
    mov byte [0xB800B], 0x0F
    mov byte [0xB800D], 0x0F
    mov byte [0xB800F], 0x0F
    mov byte [0xB8011], 0x0F
    mov byte [0xB8013], 0x0F

    mov eax, 42
    jmp halt

halt:
    cli
    hlt
