#include "types.h"
#include "uart.h"
#include "paging.h"

void kstart(void) {
    // paging_init();
    uart_init();
    
    // just a joke, will actually remove later
    uart_writes("i like eating cereal because i'm serial!\n");
    *(volatile u8*)0xB8000 = 'Z';

    for (;;) {}
}
