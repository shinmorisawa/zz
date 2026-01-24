#include <__stdarg_va_list.h>
#include <stdarg.h>
#include "types.h"
#include "utils.h"
#if CONFIG_UART
#include "uart.h"
#endif
#include "handlers.h"

void outb(u16 port, u8 v) {
    asm volatile ("outb %0, %1" : : "a"(v), "Nd"(port));
}

u8 inb(u16 port) {
    u8 ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void kputc(char c) {
    #if defined(CONFIG_UART)
    uart_write(c);
    #endif
}

void kputs(const char* s) {
    while (*s) {
        kputc(*s++);
    }
}

void print_hex(u64 base) {
    char final[17];

    for (int i = 15; i >= 0; i--) {
        final[i] = HEX[base & 0xF];
        base >>= 4;
    }

    final[16] = '\0';

    kputs(final);
}

void print_dec(u64 base) {
    char final[21];
    char temp[21];
    int i = 0;

    if (base == 0) {
        final[0] = '0';
        final[1] = '\0';
        return;
    }

    while (base > 0) {
        temp[i++] = '0' + (base % 10);
        base /= 10;
    }

    int j = 0;
    while (i > 0) {
        final[j++] = temp[--i];
    }

    final[j] = '\0';

    kputs(final);
}

void kvprintf(const char* fmt, va_list args) {
    for (; *fmt; fmt++) {
        if (*fmt != '%') {
            kputc(*fmt);
            continue;
        }

        fmt++;

        switch (*fmt) {
            case 's': {
                char* s = va_arg(args, char*);
                kputs(s ? s : "(null)");
                break;
            }

            case 'c': {
                char c = (char)va_arg(args, int);
                kputc(c);
                break;
            }

            case 'x': {
                u64 v = va_arg(args, int);
                print_hex(v);
                break;
            }

            case 'd': {
                int v = va_arg(args, int);
                print_dec(v);
                break;
            }

            case '%': {
                kputc('%');
                break;
            }

            default: {
                kputs("%?");
                break;
            }
        }
    }
}

void kprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    kvprintf(fmt, args);
    va_end(args);
}

void sleep(u64 milliseconds) {
    u64 current = ticks;
    while ((ticks - current) < milliseconds) {
        asm volatile ("hlt" : : :);
    }
    return;
}
