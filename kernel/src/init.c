#include "types.h"
#if CONFIG_UART
#include "uart.h"
#endif
#include "paging.h"
#include "interrupt.h"
#include "handlers.h"
#include "utils.h"

void halt(void) {
    asm volatile ("hlt" : : :);
}

void kstart(void) {
    paging_init();
    #if CONFIG_UART
    uart_init();
    #endif
    idt_init();
    
    // just a joke, will actually remove later
    klog("i like eating cereal because i'm serial!\n");
    *(volatile u8*)0xB8000 = 'Z';

    for (;;) {
        sleep(1);
    }
}
