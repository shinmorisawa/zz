#pragma once

#include "types.h"

extern volatile u64 ticks;

typedef struct InterruptFrame {
    u64 rcx, rdx, rsi, rdi, r8, r9, r10, r11, rax;
    u64 vector;
    u64 error_code;
    u64 rip;
    u64 cs;
    u64 rflags;
} InterruptFrame;
