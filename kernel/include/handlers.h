#pragma once

#include "types.h"

extern volatile u64 ticks;

typedef struct InterruptFrame {
    u64 rcx;
    u64 rdx;
    u64 rsi;
    u64 rdi;
    u64 r8;
    u64 r9;
    u64 r10;
    u64 r11;
    u64 rax;
    u64 vector;
    u64 error_code;
    u64 rip;
    u64 cs;
    u64 rflags;
} __attribute__((packed)) InterruptFrame;
