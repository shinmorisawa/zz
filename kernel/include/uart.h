#pragma once

#include "types.h"

void uart_init(void);
int uart_is_transmit_empty(void);
void uart_write(char c);
void uart_writes(char* s);
