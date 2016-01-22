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
	CHAR_ZERO,
	CHAR_ONE,
	CHAR_TWO,
	CHAR_THREE,
	CHAR_FOUR,
	CHAR_FIVE,
	CHAR_SIX,
	CHAR_SEVEN,
	CHAR_EIGHT,
	CHAR_NINE,
	CHAR_A,
	CHAR_V,
	CHAR_LITTLEV,
	CHAR_P,
	CHAR_H,
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
		// P
		{ .pbbits = 1, .pcbits = (1 << 7) | (1 << 4), .pdbits = (1 << 1),
				.pebits = (1 << 5) },
		// H
		{ .pbbits = 1, .pcbits = (1 << 7) | (1 << 6) | (1 << 4), .pdbits = 0,
				.pebits = (1 << 5) },
		// SPACE
		{ .pbbits = 0, .pcbits = 0, .pdbits = 0, .pebits = 0 } };

static digit digits[] = { { .odr = PD_ODR, .bit = 4 },
		{ .odr = PC_ODR, .bit = 5 }, //
		{ .odr = PC_ODR, .bit = 3 }, //
		{ .odr = PC_ODR, .bit = 2 } };

static character currentchars[4];
static int dotpos = 0;

static void setdigit(character c, bool dot) {
	*PB_ODR &= ~cbits[CHAR_EIGHT].pbbits;
	*PC_ODR &= ~cbits[CHAR_EIGHT].pcbits;
	*PD_ODR &= ~cbits[CHAR_EIGHT].pdbits;
	*PE_ODR &= ~cbits[CHAR_EIGHT].pebits;

	*PB_ODR |= cbits[c].pbbits;
	*PC_ODR |= cbits[c].pcbits;
	*PD_ODR |= cbits[c].pdbits;
	*PE_ODR |= cbits[c].pebits;

	if (dot)
		*PD_ODR |= 1;
	else
		*PD_ODR &= ~1;

}

static void turnonled() {
	*PB_ODR |= (1 << 1);
}

static void turnoffled() {
	*PB_ODR &= ~(1 << 1);
}

static int last = 0;

static void toggleled() {
	if (last == 0) {
		if (*PB_ODR & (1 << 1))
			turnoffled();
		else
			turnonled();
	}
}

void display_refresh(void)
__interrupt(INTERRUPT_TIM4) {
	character ch;
	bool dot;
	int digit = last & 0x03;
	*digits[digit].odr |= (1 << digits[digit].bit);
	last++;
	digit = last & 0x03;
	dot = (dotpos == digit);
	ch = currentchars[digit];

	switch (om) {
		case OPMODE_ON:
		turnonled();
		break;
		case OPMODE_OFF:
		turnoffled();
		break;
		case OPMODE_SET:
		if((digit == digitbeingset) && (last < 0x7f))
		ch = CHAR_SPACE;
		if(digit == 3)
		dot = (om == OPMODE_SET);
		break;
		case OPMODE_LVC:
		toggleled();
		break;
	}

	setdigit(ch, dot);
	*digits[digit].odr &= ~(1 << digits[digit].bit);

	TIM4_SR &= ~TIM4_SR_UIF;
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
		if (om == OPMODE_SET)
			value = lvc;
		else
			value = volts;
		break;
	case AMPS:
		unit = CHAR_A;
		if (om == OPMODE_SET)
			value = targetamps;
		else
			value = amps;
		break;
	case AMPHOURS:
		unit = CHAR_H;
		value = amphours;
		break;
	case WATTS:
		unit = CHAR_P;
		value = watts;
		break;
	}

	millis = value % 1000;
	units = (value - millis) / 1000;

	split(units, splittmp, 3);
	for (pos = 0; pos < 3; pos++) {
		if (splittmp[pos] != 0 || pos == 2) {
			currentchars[digit] = (character) splittmp[pos];
			if (pos == 2)
				dotpos = digit;
			digit++;
		}
	}

	split(millis, splittmp, 3);
	for (pos = 0; pos < 3 && digit < 3; pos++) {
		currentchars[digit] = (character) splittmp[pos];
		digit++;
	}

	currentchars[digit] = unit;
}

void display_init(void) {
	setuppins(PB_DDR, PB_CR1, cbits[CHAR_EIGHT].pbbits | (1 << 1));
	setuppins(PC_DDR, PC_CR1,
			(cbits[CHAR_EIGHT].pcbits | (1 << 5) | (1 << 3) | (1 << 2)));
	setuppins(PD_DDR, PD_CR1, (cbits[CHAR_EIGHT].pdbits | (1 << 4) | 1));
	setuppins(PE_DDR, PE_CR1, cbits[CHAR_EIGHT].pebits);

	*digits[0].odr |= 1 << digits[0].bit;
	*digits[1].odr |= 1 << digits[1].bit;
	*digits[2].odr |= 1 << digits[2].bit;
	*digits[3].odr |= 1 << digits[3].bit;

	TIM4_PSCR = 128;
	TIM4_EGR |= TIM1_EGR_UG;
	TIM4_IER |= TIM4_IER_UIE;
	TIM4_CR1 |= TIM4_CR1_CEN;
}
