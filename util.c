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

uint16_t pack(uint16_t* buffer, int digits) {
	uint16_t tmp = 0;
	int i;
	for (i = 0; i < digits; i++) {
		tmp *= 10;
		tmp += buffer[i];
	}
	return tmp;
}

void setuppins(volatile uint8_t* ddr, volatile uint8_t* cr1, uint8_t bits) {
	*ddr |= bits;
	*cr1 |= bits;
}
