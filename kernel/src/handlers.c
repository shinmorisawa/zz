#include "types.h"
#include "interrupt.h"
#include "handlers.h"
#include "uart.h"

volatile u64 ticks = 0;

__attribute__((interrupt))
void timer_handler(struct InterruptFrame* frame) {
    (void)frame;
    ticks++;
    pic_eoi();
}
