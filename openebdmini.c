/* Firmware to replace the original on EBD-MINI v3 programmable load
 *
 * Hardware info:
 *
 * MOSFET for the load
 * Shunt regulator for 3.3v supply
 * 2 x LM358A op-amps
 * Shunt resistor for current sensing, seems to be 20mOhm
 *
 * LED - PB1
 * Fan - PB2
 * 
 * Programming connector, pin closest to the stm8 first
 * swim, 3.3v, nrst, gnd
 *
 * AIN3 PB3  -- -A, +A end of shunt
 * AIN4 PB4 -- voltage, low gain
 * AIN5 PB5 -- voltage, high gain, handles upto ~6v
 *
 * LOAD		PC1 -- Goes into one of the op amps
 * ON 		PD3
 * SET		PD7
 *
 * Display select, left to right
 * 0 - PD4
 * 1 - PC5
 * 2 - PC3
 * 3 - PC2
 *
 * Bottom			PD2
 * Bottom Right		PC6
 * Bottom Left		PB0
 * Top				PD1
 * Top Right		PE5
 * Top Left			PC7
 * Bar				PC4
 * Dot				PD0
 */

#include "state.h"
#include "stm8.h"
#include "uart.h"
#include "display.h"
#include "util.h"
#include "buttons.h"
#include "load.h"

#define FANWATTTHRESHOLD	2500

#define ADC_VIN 4
#define ADC_SHUNT 3
#define ADC_VIN_HIGHGAIN 5
#define MILLIVOLTSPERSTEP 20

// tenths of millivolts
#define MILLIVOLTSPERSTEP_HIGHGAIN 65
#define HIGHOFFSET 20

#define MICROVOLTSPERSTEP_SHUNT 68

static uint32_t systick = 0;

static void initsystem(void) {
	*CLK_CKDIVR = 0; // default is 2MHz, remove divider for 16MHz
}

static void initfan(void) {
	setuppins(PB_DDR, PB_CR1, 1 << 2);
}

static void initserial(void) {
	uart_configure();
}

static void setadcchan(int which) {
	uint8_t csr = ADC_CSR;
	csr &= ~0b111;
	csr |= which;
	ADC_CSR = csr;
}

static void initadc(void) {
	setadcchan(ADC_VIN);
	ADC_CR1 |= ADC_CR1_ADON;
}

static void turnonfan() {
	*PB_ODR |= (1 << 2);
}
static void turnofffan() {
	*PB_ODR &= ~(1 << 2);
}

static void configureload() {
	*PC_ODR |= 1 << 1;
}

static uint16_t readadc(int which) {
	uint16_t result;
	setadcchan(which);
	ADC_CR1 |= ADC_CR1_ADON;
	while ((ADC_CSR & ADC_CSR_EOC) == 0) {

	}
	ADC_CSR &= ~ADC_CSR_EOC;
	result = (((uint16_t) ADC_DRH) << 2) | ADC_DRL;
	return result;
}

static int8_t volttrim = -1;

#define SAMPLES 12
static uint16_t voltsamples[SAMPLES] = { 0 };
static uint16_t shuntsamples[SAMPLES] = { 0 };
static uint16_t volthighgainsamples[SAMPLES] = { 0 };
static uint8_t sample = 0;

static void checkstate(void) {
	int i;
	uint32_t voltsum = 0;
	uint32_t shuntsum = 0;
	uint32_t volthighgainsum = 0;
	uint32_t wattstemp = 0;
	uint16_t lowgainvolts, highgainvolts;

	shuntsamples[sample] = readadc(ADC_SHUNT);
	voltsamples[sample] = readadc(ADC_VIN);
	volthighgainsamples[sample] = readadc(ADC_VIN_HIGHGAIN);

	sample = (sample + 1) % SAMPLES;
	for (i = 0; i < SAMPLES; i++) {
		shuntsum += shuntsamples[i];
		voltsum += voltsamples[i];
		volthighgainsum += volthighgainsamples[i];
	}

	amps = ((shuntsum / SAMPLES) * MICROVOLTSPERSTEP_SHUNT) / 20;
	highgainvolts = (((volthighgainsum / SAMPLES) * MILLIVOLTSPERSTEP_HIGHGAIN)
			/ 10) - HIGHOFFSET;
	lowgainvolts = (voltsum / SAMPLES) * (MILLIVOLTSPERSTEP);
	highgain = lowgainvolts <= 6000;
	if (highgain)
		volts = highgainvolts;
	else
		volts = lowgainvolts;

	wattstemp = ((uint32_t) amps * (uint32_t) volts) / 1000;
	watts = (uint16_t) wattstemp;

	if (watts > FANWATTTHRESHOLD)
		turnonfan();
	else
		turnofffan();

	switch (om) {
	case OPMODE_ON:
		if (volts < lvc) {
			om = OPMODE_LVC;
			load_turnoff();
		} else {
			if (amps < targetamps) {
				if (loadduty > 300) {
					loadduty--;
					load_setduty(loadduty);
				}
			} else if (amps > targetamps) {
				if (loadduty < 1000) {
					loadduty++;
					load_setduty(loadduty);
				}
			}
		}
		break;
	case OPMODE_OFF:
		load_turnoff();
		break;
	}
}

static void sep(void) {
	uart_puts(",");
}

static void splitandprintvalue(uint16_t value) {
	int i;
	uint16_t splittmp[6];
	split(value, splittmp, 6);
	for (i = 0; i < 6; i++) {
		uart_putch(splittmp[i] + 0x30);
	}
}

static void sendstate(void) {

	switch (om) {
	case OPMODE_OFF:
		uart_puts("off");
		break;
	case OPMODE_SET:
		uart_puts("set");
		break;
	case OPMODE_ON:
		uart_puts("on");
		break;
	case OPMODE_LVC:
		uart_puts("lvc");
		break;
	}
	sep();

	// volts
	splitandprintvalue(volts);
	sep();

	// amps
	splitandprintvalue(amps);
	sep();

	// watts
	splitandprintvalue(watts);
	sep();

	// target amps
	splitandprintvalue(targetamps);
	sep();

	// lvc
	splitandprintvalue(lvc);

	//
	sep();
	splitandprintvalue(loadduty);

	//
	sep();
	splitandprintvalue(TIM2_CNTRL);

	uart_puts("\r\n");
}

int main() {
	load_init();
	initsystem();
	initserial();
	display_init();
	initfan();
	initadc();

	buttons_init();

	enableInterrupts();

	while (1) {
		buttons_check();
		checkstate();
		sendstate();
		display_update();
	}
}
