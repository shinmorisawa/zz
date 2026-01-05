#include "types.h"

void kstart(void) {
    volatile u16* vga = (volatile u16*)0xB8000;
    vga[0] = (0xF0 << 8) | 'K';
    vga[1] = (0xF0 << 8) | 'E';
    vga[2] = (0xF0 << 8) | 'R';
    vga[3] = (0xF0 << 8) | 'N';
    vga[4] = (0xF0 << 8) | 'E';
    vga[5] = (0xF0 << 8) | 'L';

    for (;;) {}
}
