#pragma once

#include <stdarg.h>

#include "types.h"

#define HEX "0123456789ABCDEF"
#define DEC "0123456789"

__attribute__((no_caller_saved_registers))
void outb(u16 port, u8 v);
__attribute__((no_caller_saved_registers))
u8 inb(u16 port);

void kputc(char c);
void kputs(const char* s);
void print_hex(u64 base);
void print_dec(u64 base);
void kvprintf(const char* fmt, va_list args);
void kprintf(const char* fmt, ...);
void sleep(u64 milliseconds);
void kpanic(void);

#define klog kprintf
