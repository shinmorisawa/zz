#pragma once

#include "types.h"

extern volatile u64 ticks;

struct InterruptFrame {
    u64 rip;
    u64 cs;
    u64 flags;
    u64 rsp;
    u64 ss;
};

__attribute__((interrupt))
void timer_handler(struct InterruptFrame* frame);
