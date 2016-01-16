/* Firmware to replace the original on EBD-MINI v3 programmagble load
 *
 * Hardware info:
 * LED - PB1
 * Fan - PB2
 * 

      AIN3 PB3
 	AIN4 PB4


* ON 		PD3
 * SET		PD7
 *
 * Left to right
 * 0 - PD4
 * 1 - PC5
 * 2 - PC3
 * 3 - PC2
 *
 * Bottom	PD2 --
 * Bottom Right	PC6 --
 * Bottom Left	PB0 --
 * Top 		PD1 --
 * Top Right	PE5 --
 * Top Left	PC7
 * Bar		PC4
 * Dot		PD0
 */

#include <stdbool.h>

#include "stm8.h"

typedef struct {
	volatile uint8_t* odr;
	const int bit;
} digit;

typedef enum {
	ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, A, V
} character;

typedef struct {
	const uint8_t pbbits;
	const uint8_t pcbits;
	const uint8_t pdbits;
	const uint8_t pebits;
} characterbits;

static characterbits cbits[] = {
	// ZERO
	{ 
	.pbbits = 1,
	.pcbits = (1 << 7) | (1<< 6), 
	.pdbits = (1 << 2) | (1 << 1), 
	.pebits = (1 << 5)},
        // ONE
        { 
        .pbbits = 1,
        .pcbits = (1 << 7), 
        .pdbits = 0, 
        .pebits = 0},
        // TWO
        { 
        .pbbits = 1,
        .pcbits = (1 << 4), 
        .pdbits = (1 << 2) | (1 << 1), 
        .pebits = (1 << 5)},
        // THREE
        { 
        .pbbits = 0,
        .pcbits = (1<< 6) | (1 << 4), 
        .pdbits = (1 << 2) | (1 << 1), 
        .pebits = 1 << 5},
        // FOUR
        { 
        .pbbits = 0,
        .pcbits = (1 << 7) | (1<< 6) | (1 << 4), 
        .pdbits = 0, 
        .pebits = (1 << 5)},
        // FIVE
        { 
        .pbbits = 0,
        .pcbits = (1 << 7) | (1<< 6) | (1 << 4), 
        .pdbits = (1 << 2) | (1 << 1), 
        .pebits = 0},
        // SIX
        { 
        .pbbits = 1,
        .pcbits = (1 << 7) | (1<< 6) | (1 << 4), 
        .pdbits = (1 << 2) | (1 << 1), 
        .pebits = 0},
        // SEVEN
        { 
        .pbbits = 0,
        .pcbits = (1<< 6), 
        .pdbits = (1 << 1), 
        .pebits = (1 << 5)},
        // EIGHT
        { 
        .pbbits = 1,
        .pcbits = (1 << 7) | (1<< 6) | (1 << 4), 
        .pdbits = (1 << 2) | (1 << 1), 
        .pebits = (1 << 5)},
	// NINE
        { 
        .pbbits = 0,
        .pcbits = (1 << 7) | (1<< 6) | (1 << 4), 
        .pdbits = (1 << 2) | (1 << 1), 
        .pebits = (1 << 5)},
        // A
        { 
        .pbbits = 1,
        .pcbits = (1 << 7) | (1<< 6) | (1 << 4), 
        .pdbits = (1 << 1), 
        .pebits = (1 << 5)},
        // V
        { 
        .pbbits = 1,
        .pcbits = (1 << 7) | (1<< 6),
        .pdbits = (1 << 2), 
        .pebits = (1 << 5)}
};

static digit digits[] = {
{.odr = PD_ODR, .bit = 4},
{.odr = PC_ODR, .bit = 5},
{.odr = PC_ODR, .bit = 3},
{.odr = PC_ODR, .bit = 2}};





static void setdigit(int which, character c, bool dot){
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
	if(dot)
		*PD_ODR |= 1;
	else
		*PD_ODR &= ~1;

	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){}
	}
	
	*digits[which].odr |= 1 << digits[which].bit;
}

static void turnonled(){
	*PB_ODR |= (1 << 1);
}

static void turnonfan(){
	*PB_ODR |= (1 << 2);
}

int main() {
	int d;

//	*PB_CR1 = (1 << 2) | (1 << 1);
//	*PB_DDR = (1 << 2) | (1 << 1);

	*PB_CR1 = 0xff;
	*PB_DDR = 0xff;

	*PC_CR1 = 0xff;
	*PC_DDR = 0xff;

//	*PA_CR1 = 0xff;
//	*PA_DDR = 0xff;
//	*PA_ODR = 0xff;

	*PD_CR1 = 0xff;
	*PD_DDR = 0xff;
//	*PD_ODR = 0xff;
	

	turnonled();
	turnonfan();

	*PE_CR1 = (1 << 5);
	*PE_DDR = (1 << 5);
	*PE_ODR = (1 << 5);

	while (1) {
		for(d = 0; d < 4; d++){
			setdigit(d, d + 8, false);
		}
		*PC_ODR |= (1 << 4);
	}
}
