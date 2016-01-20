#include <stdbool.h>

#include "stm8.h"
#include "uart.h"

static char txbuffer[16];
static uint8_t txhead = 0;
static uint8_t txtail = 0;

void uart_txhandler(void)
__interrupt(INTERRUPT_UART2_TXCOMPLETE) {
	UART2_SR &= ~UART2_SR_TC;
	if(txtail != txhead) {
		UART2_DR = txbuffer[txtail];
		txtail = (txtail + 1) % 16;
	}
}

void uart_rxhandler(void)
__interrupt( INTERRUPT_UART2_RXFULL) {

}

void uart_putch(char ch) {
	if (txtail == txhead && (UART2_SR & UART2_SR_TXE) != 0) {
		UART2_DR = ch;
	} else {
		while (((txtail + 1) % 16) == txhead) {
			waitforinterrupt();
		}
		txbuffer[txhead] = ch;
		txhead = (txhead + 1) % 16;
	}
}

void uart_puts(char* str) {
	while (*str != '\0')
		uart_putch(*str++);
}

void uart_getch() {

}

void uart_configure() {
	UART2_BRR1 = 0x34;
	UART2_BRR2 = 0x01;
	UART2_CR2 |= (UART2_CR2_TCIEN | UART2_CR2_TEN | UART2_CR2_REN);

}
