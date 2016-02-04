#include <stdbool.h>

#include "stm8.h"
#include "uart.h"
#include "state.h"

#define FIFOSIZE 64

static char txbuffer[FIFOSIZE];
static uint8_t txhead = 0;
static uint8_t txtail = 0;
static char rxbuffer[FIFOSIZE];
static uint8_t rxhead = 0;
static uint8_t rxtail = 0;
static bool rxoverflow = false;

void uart_txhandler(void)
__interrupt(INTERRUPT_UART2_TXCOMPLETE) {
	UART2_SR &= ~UART2_SR_TC;
	if(txtail != txhead) {
		UART2_DR = txbuffer[txtail];
		txtail = (txtail + 1) % FIFOSIZE;
	}
}

static inline void uart_rxhandler_body(void) {
	uint8_t byte = UART2_DR;
	if (!rxoverflow) {
		if (((rxhead + 1) % FIFOSIZE) != rxtail) {
			if (byte == '\n')
				sequence++;
			rxbuffer[rxhead] = byte;
			rxhead = (rxhead + 1) % FIFOSIZE;
		} else
			rxoverflow = true;
	}
}

void uart_rxhandler(void)
__interrupt( INTERRUPT_UART2_RXFULL) {
	uart_rxhandler_body();
}

void uart_putch(char ch) {
	if (txtail == txhead && (UART2_SR & UART2_SR_TXE) != 0) {
		UART2_DR = ch;
	} else {
		while (((txtail + 1) % FIFOSIZE) == txhead) {
		}
		txbuffer[txhead] = ch;
		txhead = (txhead + 1) % FIFOSIZE;
	}
}

void uart_puts(char* str) {
	while (*str != '\0')
		uart_putch(*str++);
}

bool uart_getch(uint8_t* result) {
	if (rxtail != rxhead) {
		*result = rxbuffer[rxtail];
		rxtail = (rxtail + 1) % FIFOSIZE;
	} else
		return false;
}

void uart_configure() {
	UART2_BRR1 = 0x34;
	UART2_BRR2 = 0x01;
	UART2_CR2 |= (UART2_CR2_TCIEN | UART2_CR2_RIEN | UART2_CR2_TEN
			| UART2_CR2_REN);

}
