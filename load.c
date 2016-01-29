#include "load.h"
#include "util.h"
#include "stm8.h"
#include "state.h"

#define OFFDUTY 1024

static void load_reallysetduty(uint16_t duty) {
	loadduty = duty;
	TIM1_CCR1H = (uint8_t)((duty >> 8) & 0xff);
	TIM1_CCR1L = (uint8_t)(duty & 0xff);
	TIM1_EGR |= TIM1_EGR_UG;
}

void load_setduty(uint16_t duty) {
	if (duty > 300 && duty < OFFDUTY) {
		load_reallysetduty(duty);
	}
}

void load_turnoff(void) {
	load_reallysetduty(OFFDUTY);
}

void load_init(void) {
	const uint16_t reloadvalue = OFFDUTY;

	setuppins(PC_DDR, PC_CR1, 1 << 1);
	*PC_ODR &= ~(1 << 1);

	TIM1_CCMR1 |= (111 << 4); // pwm mode 1
	TIM1_CCER1 |= TIM1_CCER1_CC1E;
	TIM1_BKR |= TIM1_BKR_MOE;

	TIM1_ARRH = (uint8_t)((reloadvalue >> 8) & 0xff);
	TIM1_ARRL = (uint8_t)(reloadvalue & 0xff);

	load_turnoff();
	TIM1_CR1 |= TIM1_CR1_ARPE | TIM1_CR1_CEN;
}
