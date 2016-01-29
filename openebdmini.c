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
#include "adc.h"
#include "timer.h"

#define FANWATTTHRESHOLD	2500

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

static void turnonfan() {
	*PB_ODR |= (1 << 2);
}
static void turnofffan() {
	*PB_ODR &= ~(1 << 2);
}

static int8_t volttrim = -1;

static void checkstate(void) {
	static operationmode lastmode = OPMODE_OFF;

	// stuff that only happens at mode changes
	if (om != lastmode) {
		switch (om) {
		case OPMODE_ON:
			timer_start();
			break;
		case OPMODE_LVC:
		case OPMODE_OFF:
			load_turnoff();
			timer_stop();
			turnofffan();
			break;
		}
		lastmode = om;
	}

	// stuff that always has to happen while the load is on
	if (om == OPMODE_ON) {
		if (volts < lvc) {
			load_turnoff();
			om = OPMODE_LVC;
		} else {
			if (amps < targetamps) {
				load_setduty(loadduty - 1);
			} else if (amps > targetamps) {
				load_setduty(loadduty + 1);
			}
		}
	}

	if (watts > FANWATTTHRESHOLD)
		turnonfan();
	else
		turnofffan();
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

	uart_puts("\r\n");
}

int main() {
	bool newreadings;
	bool buttonschanged;

	load_init();
	initsystem();
	initserial();
	timer_init();
	display_init();
	initfan();
	adc_init();

	buttons_init();

	enableInterrupts();

	while (1) {
		newreadings = adc_updatereadings();
		if (newreadings) {
			checkstate();
		}
		buttonschanged = buttons_check();
		if (newreadings || buttonschanged) {
			sendstate();
			display_update();
		}
	}
}
