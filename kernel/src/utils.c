#include "types.h"
#include "utils.h"

void outb(u16 port, u8 v) {
    asm volatile ("outb %0, %1" : : "a"(v), "Nd"(port));
}

u8 inb(u16 port) {
    u8 ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
