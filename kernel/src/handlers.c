#include "types.h"
#include "interrupt.h"
#include "handlers.h"
#include "utils.h"
#include "keyboard.h"

volatile u64 ticks = 0;

void interrupt_handler(InterruptFrame* frame) {
    switch (frame->vector) {
        case 0x0:
            klog("div error lmao\n");
            break;
        case 0xE:
            u64 fault_addr;
            asm volatile ("mov %%cr2, %0" : "=r"(fault_addr));

            u64 error = frame->error_code;
            u64 rip = frame->rip;

            klog("\n-- page fault --\n");
            klog("  fault addr: 0x%x\n", fault_addr);
            klog("  rip:        0x%x\n", rip);
            klog("  error code: 0x%x (P:%d W/R:%d U/S:%d RSVD:%d ID:%d)\n", error, (error & 1) ? 1 : 0, (error & 2) ? 1 : 0, (error & 4) ? 1 : 0, (error & 8) ? 1 : 0, (error & 16) ? 1 : 0);
            break;
        case 0x20:
            ticks++;
            pic_eoi();
            break;
        default:
            klog("unknown, returning\n");
            break;
    }
}
