#include "buttons.h"
#include "state.h"
#include "util.h"

static uint16_t setdown = 0;
static uint16_t ondown = 0;
static volatile presstype setpt = PT_NONE;
static volatile presstype onpt = PT_NONE;

static inline uint16_t buttons_gettime() {
	return ((TIM2_CNTRH << 8) | (TIM2_CNTRL)) & 0x7fff;
}

static inline presstype buttons_getpresstype(uint16_t downtime) {
	uint16_t now = buttons_gettime();
	uint16_t timedown;
	if (now < downtime)
		downtime |= 1 << 15;

	timedown = now - downtime;
	if (timedown > 1000)
		return PT_LONG;
	else if (timedown > 50)
		return PT_SHORT;
	else
		return PT_NONE;
}

void buttons_set_handler(void)
__interrupt( INTERRUPT_TLI) {
	if(setpt == PT_NONE) {
		setpt = PT_DOWN;
		setdown = buttons_gettime();
	}
}

void buttons_on_handler(void)
__interrupt(INTERRUPT_EXTI3) {
	if(onpt == PT_NONE) {
		onpt = PT_DOWN;
		ondown = buttons_gettime();
	}
}

static void buttons_incdisplaymode(void) {
	dm++;
	if (dm == DISPMODE_END || ((om == OPMODE_SET) && dm == AMPHOURS))
		dm = 0;
}

static void buttons_incselecteddigit(void) {
	digitbeingset = (digitbeingset + 1) % 3;
}

static void buttons_incvalue() {
	uint16_t tmp[4];
	split(targetamps, tmp, 4);
	tmp[digitbeingset] = (tmp[digitbeingset] + 1) % 10;
	targetamps = pack(tmp, 4);
}

void buttons_check(void) {
	uint8_t buttonstate = *PD_IDR;
	bool onup = (buttonstate & (1 << 3));
	bool checkon = (onpt == PT_DOWN) && onup;
	bool setup = (buttonstate & (1 << 7));
	bool checkset = (setpt == PT_DOWN) && setup;

	if (checkon) {
		onpt = buttons_getpresstype(ondown);
		switch (om) {
		case OPMODE_OFF:
			om = OPMODE_ON;
			break;
		case OPMODE_ON:
			om = OPMODE_OFF;
			break;
		case OPMODE_SET:
			if (onpt == PT_LONG)
				buttons_incselecteddigit();
			else
				buttons_incvalue();
			break;
		}
		onpt = PT_NONE;
	}

	if (checkset) {
		setpt = buttons_getpresstype(setdown);
		switch (om) {
		case OPMODE_ON:
			buttons_incdisplaymode();
			break;
		case OPMODE_OFF:
			if (setpt == PT_LONG) {
				dm = AMPS;
				om = OPMODE_SET;
			} else
				buttons_incdisplaymode();
			break;
		case OPMODE_SET:
			if (setpt == PT_LONG)
				om = OPMODE_OFF;
			else
				buttons_incdisplaymode();
			break;
		case OPMODE_LVC:
			if (setpt == PT_LONG)
				om = OPMODE_OFF;
			break;
		}
		setpt = PT_NONE;
	}

}

void buttons_init(void) {
	const uint8_t mask = (1 << 3) | (1 << 7);

	TIM2_PSCR |= 0x0f;
	TIM2_CR1 |= TIM2_CR1_CEN;

	EXTI_CR1 |= EXTI_CR1_PDIS_FALLING;

	*PD_DDR &= ~mask; // input
	*PD_CR1 |= mask; // pull up
	*PD_CR2 |= mask; // interrupts
}
