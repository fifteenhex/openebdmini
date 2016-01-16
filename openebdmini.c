/* Firmware to replace the original on EBD-MINI v3 programmagble load
 *
 * Hardware info:
 * LED - PB1
 * Fan - PB2
 * 

      AIN3 PB3
 	AIN4 PB4 -- Voltage?

 * PC1 
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
	ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, CHAR_A, CHAR_V, CHAR_SPACE
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
        .pebits = (1 << 5)},
	// SPACE
	{   
        .pbbits = 0,
        .pcbits = 0,
        .pdbits = 0, 
        .pebits = 0}
};

static digit digits[] = {
{.odr = PD_ODR, .bit = 4},
{.odr = PC_ODR, .bit = 5},
{.odr = PC_ODR, .bit = 3},
{.odr = PC_ODR, .bit = 2}};

typedef enum {
	OPMODE_OFF, OPMODE_SET, OPMODE_ON
} operationmode;

typedef enum {
 VOLTS, AMPS, AMPHOURS, WATTS
} displaymode;

static operationmode om = OPMODE_OFF;
static displaymode dm = VOLTS;

static uint32_t systick = 0;

static uint16_t lvc = 2000;

static uint16_t volts = 0;
static uint16_t amps = 0;
static uint16_t amphours = 0;
static uint16_t watts = 0;

static void setuppins(volatile uint8_t* ddr, volatile uint8_t* cr1, uint8_t bits){
	*ddr |= bits;
	*cr1 |= bits;
}

static void initsystem(void){
	*CLK_CKDIVR = 0; // default is 2MHz, remove divider for 16MHz
}

static void initload(void){
	setuppins(PC_DDR, PC_CR1, 1 << 1);
	*PC_ODR &= 1 << 1;
}

static void initfan(void){
	setuppins(PB_DDR, PB_CR1, 1 << 2);
}

static void initserial(void){

}

static void initdisplay(void){
	setuppins(PB_DDR, PB_CR1, cbits[EIGHT].pbbits | ( 1 << 1));
        setuppins(PC_DDR, PC_CR1, (cbits[EIGHT].pcbits | (1 << 5) | (1 << 3) | (1 << 2)));
       	setuppins(PD_DDR, PD_CR1, (cbits[EIGHT].pdbits | (1 << 4) | 1));
	setuppins(PE_DDR, PE_CR1, cbits[EIGHT].pebits);
}

static void initbuttons(void){
	uint8_t mask = (1 << 3) | (1 << 7);
	*PD_DDR &= ~mask; // input
	*PD_CR1 |= mask; // pull up
}

static void initadc(void){
	ADC_CR1 |= ADC_CR1_ADON;
}

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

	for(i = 0; i < 32; i++){
		for(j = 0; j < 32; j++){}
	}
	
	*digits[which].odr |= 1 << digits[which].bit;

	for(i = 0; i < 32; i++){
		for(j = 0; j < 32; j++){}
	}

}

static void turnonled(){
	*PB_ODR |= (1 << 1);
}

static void turnonfan(){
	*PB_ODR |= (1 << 2);
}

static void split(uint16_t value, uint16_t* buffer){
	buffer[2] = value % 10;
	value -= buffer[2];
	buffer[1] = value % 100;
	value -= buffer[1];
	buffer[0] = value;

	buffer[1] /= 10;
	buffer[0] /= 100;
}

static uint16_t readadc(int which){
	uint16_t result; 
	ADC_CSR |= which;
	ADC_CR1 |= ADC_CR1_ADON;
	while((ADC_CSR & ADC_CSR_EOC) == 0){

	}
	ADC_CSR &= ~ADC_CSR_EOC;
	result = (((uint16_t) ADC_DRH) << 2) | ADC_DRL; 
	return result;
}

static void checkstate(void){
	//volts++;
	//volts = readadc(3);
	volts = readadc(4);
	//volts = readadc(5);
}

static void checkbuttons(void){
	uint8_t portbits = *PD_IDR;
	if((portbits & (1 << 3)) == 0)
		volts += 100;
	if((portbits & (1 << 7)) == 0)
		volts -= 100;
}

static void updatedisplay(void){
	uint16_t millis;
	uint16_t units;

	uint16_t	value = 1;
	character	unit;

	uint16_t splittmp[3];
	int digit = 0;
	int pos;

	switch(dm){
		case VOLTS:
			unit = CHAR_V;
			value = volts;
			break;
		case AMPS:
			unit = CHAR_A;
			break;
		case AMPHOURS:
			unit = CHAR_A;
			break;
		case WATTS:
			unit = CHAR_A;
			break;
	}
	

	millis = value % 1000;
	units = (value - millis) / 1000;
	
	split(units, splittmp);

	for(pos = 0; pos < 3; pos++){
		if(splittmp[pos] != 0 || pos == 2){
			setdigit(digit, (character) splittmp[pos], pos == 2);
			digit++;
		}
	}

	split(millis, splittmp);
	for(pos = 0; pos < 3 && digit < 3; pos++){
		setdigit(digit, (character) splittmp[pos], false);
		digit++;
	}

	

	setdigit(digit, unit, false);

}

int main() {
	initload();
	initsystem();
	initserial();
	initdisplay();
	initbuttons();
	initfan();
	initadc();

	turnonled();
//	turnonfan();

	while (1) {
		checkstate();
		checkbuttons();
		updatedisplay();
	}
}
