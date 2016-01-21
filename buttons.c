#include "buttons.h"
#include "state.h"

void buttons_set_handler(void)
__interrupt( INTERRUPT_TLI) {
	*PB_ODR |= (1 << 1);
}

void buttons_on_handler(void)
__interrupt(INTERRUPT_EXTI3) {
	//*PB_ODR |= (1 << 1);
}

void buttons_check(void) {
	uint8_t portbits = *PD_IDR;

	if ((portbits & (1 << 3)) == 0) {
		switch (om) {
		case OPMODE_OFF:
			om = OPMODE_ON;
			break;
		case OPMODE_ON:
			om = OPMODE_OFF;
			break;
		}
	}

	if ((portbits & (1 << 7)) == 0) {
		switch (om) {
		case OPMODE_ON:
		case OPMODE_OFF:
			dm++;
			if (dm == DISPMODE_END)
				dm = 0;
			break;
		case OPMODE_LVC:
			om = OPMODE_OFF;
			break;
		}
	}

}

void buttons_init(void) {
	uint8_t mask = (1 << 3) | (1 << 7);
	*PD_DDR &= ~mask; // input
	*PD_CR1 |= mask; // pull up
	*PD_CR2 |= mask; // interrupts
	EXTI_CR1 |= EXTI_CR1_PDIS_RISINGFALLING;
}
