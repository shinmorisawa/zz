#include "types.h"
#include "utils.h"
#include "uart.h"

#define COM1 0x3F8

void uart_init(void) {
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x80);
    outb(COM1 + 0, 0x01);
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);
    outb(COM1 + 2, 0xC7);
    outb(COM1 + 4, 0x0B);

    uart_writes("zzkern\n");
}

int uart_is_transmit_empty(void) {
    return inb(COM1 + 5) & 0x20;
}

void uart_write(char c) {
    while (!uart_is_transmit_empty());
    outb(COM1, c);
}

void uart_writes(char* s) {
    while (*s != '\0') {
        uart_write(*s);
        s++;
    }
}
