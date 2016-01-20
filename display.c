#include <stdbool.h>

#include "stm8.h"
#include "display.h"
#include "util.h"
#include "state.h"

typedef struct {
	volatile uint8_t* odr;
	const int bit;
} digit;

typedef enum {
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	CHAR_A,
	CHAR_V,
	CHAR_LITTLEV,
	CHAR_SPACE
} character;

typedef struct {
	const uint8_t pbbits;
	const uint8_t pcbits;
	const uint8_t pdbits;
	const uint8_t pebits;
} characterbits;

static characterbits cbits[] = {
// ZERO
		{ .pbbits = 1, .pcbits = (1 << 7) | (1 << 6), .pdbits = (1 << 2)
				| (1 << 1), .pebits = (1 << 5) },
		// ONE
		{ .pbbits = 1, .pcbits = (1 << 7), .pdbits = 0, .pebits = 0 },
		// TWO
		{ .pbbits = 1, .pcbits = (1 << 4), .pdbits = (1 << 2) | (1 << 1),
				.pebits = (1 << 5) },
		// THREE
		{ .pbbits = 0, .pcbits = (1 << 6) | (1 << 4), .pdbits = (1 << 2)
				| (1 << 1), .pebits = 1 << 5 },
		// FOUR
		{ .pbbits = 0, .pcbits = (1 << 7) | (1 << 6) | (1 << 4), .pdbits = 0,
				.pebits = (1 << 5) },
		// FIVE
		{ .pbbits = 0, .pcbits = (1 << 7) | (1 << 6) | (1 << 4), .pdbits = (1
				<< 2) | (1 << 1), .pebits = 0 },
		// SIX
		{ .pbbits = 1, .pcbits = (1 << 7) | (1 << 6) | (1 << 4), .pdbits = (1
				<< 2) | (1 << 1), .pebits = 0 },
		// SEVEN
		{ .pbbits = 0, .pcbits = (1 << 6), .pdbits = (1 << 1), .pebits =
				(1 << 5) },
		// EIGHT
		{ .pbbits = 1, .pcbits = (1 << 7) | (1 << 6) | (1 << 4), .pdbits = (1
				<< 2) | (1 << 1), .pebits = (1 << 5) },
		// NINE
		{ .pbbits = 0, .pcbits = (1 << 7) | (1 << 6) | (1 << 4), .pdbits = (1
				<< 2) | (1 << 1), .pebits = (1 << 5) },
		// A
		{ .pbbits = 1, .pcbits = (1 << 7) | (1 << 6) | (1 << 4), .pdbits = (1
				<< 1), .pebits = (1 << 5) },
		// V
		{ .pbbits = 1, .pcbits = (1 << 7) | (1 << 6), .pdbits = (1 << 2),
				.pebits = (1 << 5) },
		// Little V
		{ .pbbits = 1, .pcbits = (1 << 6), .pdbits = (1 << 2), .pebits = 0 },
		// SPACE
		{ .pbbits = 0, .pcbits = 0, .pdbits = 0, .pebits = 0 } };

static digit digits[] = { { .odr = PD_ODR, .bit = 4 },
		{ .odr = PC_ODR, .bit = 5 }, { .odr = PC_ODR, .bit = 3 }, { .odr =
		PC_ODR, .bit = 2 } };

static void setdigit(int which, character c, bool dot) {
	int i, j;

	*digits[which].odr &= ~(1 << digits[which].bit);

	*PB_ODR &= ~cbits[EIGHT].pbbits;
	*PC_ODR &= ~cbits[EIGHT].pcbits;
	*PD_ODR &= ~cbits[EIGHT].pdbits;
	*PE_ODR &= ~cbits[EIGHT].pebits;

	*PB_ODR |= cbits[c].pbbits;
	*PC_ODR |= cbits[c].pcbits;
	*PD_ODR |= cbits[c].pdbits;
	*PE_ODR |= cbits[c].pebits;
	if (dot)
		*PD_ODR |= 1;
	else
		*PD_ODR &= ~1;

	for (i = 0; i < 32; i++) {
		for (j = 0; j < 32; j++) {
		}
	}

	*digits[which].odr |= 1 << digits[which].bit;

}

void display_update(void) {
	uint16_t millis;
	uint16_t units;

	uint16_t value = 1;
	character unit;

	uint16_t splittmp[3];
	int digit = 0;
	int pos;

	switch (dm) {
	case VOLTS:
		unit = highgain ? CHAR_LITTLEV : CHAR_V;
		value = volts;
		break;
	case AMPS:
		unit = CHAR_A;
		value = amps;
		break;
	case AMPHOURS:
		unit = CHAR_SPACE;
		break;
	case WATTS:
		unit = CHAR_SPACE;
		break;
	}

	millis = value % 1000;
	units = (value - millis) / 1000;

	split(units, splittmp, 3);

	for (pos = 0; pos < 3; pos++) {
		if (splittmp[pos] != 0 || pos == 2) {
			setdigit(digit, (character) splittmp[pos], pos == 2);
			digit++;
		}
	}

	split(millis, splittmp, 3);
	for (pos = 0; pos < 3 && digit < 3; pos++) {
		setdigit(digit, (character) splittmp[pos], false);
		digit++;
	}

	setdigit(digit, unit, false);
}

void display_init(void) {
	setuppins(PB_DDR, PB_CR1, cbits[EIGHT].pbbits | (1 << 1));
	setuppins(PC_DDR, PC_CR1,
			(cbits[EIGHT].pcbits | (1 << 5) | (1 << 3) | (1 << 2)));
	setuppins(PD_DDR, PD_CR1, (cbits[EIGHT].pdbits | (1 << 4) | 1));
	setuppins(PE_DDR, PE_CR1, cbits[EIGHT].pebits);
}
