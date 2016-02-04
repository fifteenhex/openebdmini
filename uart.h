#pragma once

#include <stdbool.h>

#include "stm8.h"

void uart_configure(void);
void uart_putch(char ch);
void uart_puts(char* str);
bool uart_getch(uint8_t* result);

void uart_txhandler(void)
__interrupt( INTERRUPT_UART2_TXCOMPLETE);
void uart_rxhandler(void)
__interrupt( INTERRUPT_UART2_RXFULL);
