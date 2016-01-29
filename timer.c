#include "timer.h"
#include "state.h"

void timer_interrupthandler(void)
__interrupt(INTERRUPT_TIM3_UPDATEOVERFLOW) {
	time++;
	TIM3_SR1 &= ~TIM3_SR1_UIF;
}

void timer_start() {
	time = 0;
	TIM3_CNTRH = 0;
	TIM3_CCR1L = 0;
	TIM3_CR1 |= TIM3_CR1_CEN;
}

void timer_stop() {
	TIM3_CR1 &= ~TIM3_CR1_CEN;
}

void timer_init() {
	TIM3_ARRH = 0x01;
	TIM3_ARRL = 0xe8;
	TIM3_PSCR = 0x0f;
	TIM3_IER |= TIM3_IER_UIE;
}
