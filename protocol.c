#include "protocol.h"
#include "uart.h"
#include "state.h"
#include "util.h"

static inline void sep(void) {
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

void protocol_onbooted(void) {
	uart_puts("BOOTED");
}

void protocol_sendstate(void) {

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
