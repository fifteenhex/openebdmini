#include "util.h"

void split(uint16_t value, uint16_t* buffer, int digits) {
	uint16_t tmp;
	for (; digits > 0; digits--) {
		tmp = value % 10;
		buffer[digits - 1] = tmp;
		value -= tmp;
		if (value == 0) {
			digits--;
			break;
		} else
			value /= 10;
	}
	for (; digits > 0; digits--)
		buffer[digits - 1] = 0;
}

void setuppins(volatile uint8_t* ddr, volatile uint8_t* cr1, uint8_t bits) {
	*ddr |= bits;
	*cr1 |= bits;
}
