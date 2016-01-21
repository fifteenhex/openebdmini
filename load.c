#include "load.h"
#include "util.h"
#include "stm8.h"
#include "state.h"

void load_setduty(uint16_t duty) {
	TIM1_CCR1H = (uint8_t)((duty >> 8) & 0xff);
	TIM1_CCR1L = (uint8_t)(duty & 0xff);
	TIM1_EGR |= TIM1_EGR_UG;
}

void load_turnoff(void) {
	loadduty = 1025;
	load_setduty(loadduty);
}

void load_init(void) {
	const uint16_t reloadvalue = 1024;

	setuppins(PC_DDR, PC_CR1, 1 << 1);
	*PC_ODR &= ~(1 << 1);

	TIM1_CCMR1 |= (111 << 4); // pwm mode 1
	TIM1_CCER1 |= TIM1_CCER1_CC1E;
	TIM1_BKR |= TIM1_BKR_MOE;

	TIM1_ARRH = (uint8_t)((reloadvalue >> 8) & 0xff);
	TIM1_ARRL = (uint8_t)(reloadvalue & 0xff);

	load_setduty(reloadvalue);

	TIM1_CR1 |= TIM1_CR1_ARPE | TIM1_CR1_CEN;
}
