#include "protocol.h"
#include "uart.h"
#include "state.h"
#include "util.h"

static char command[16];

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

static void protocol_commanderror() {
	uart_puts("!\n");
}

static void protocol_handlecommand(protocol_command cmd) {
	switch (cmd) {
	case PROTOCOL_COMMAND_ON:
		if (state_changeopmode(OPMODE_ON))
			uart_puts("on\n");
		else
			protocol_commanderror();
		break;
	case PROTOCOL_COMMAND_OFF:
		if (state_changeopmode(OPMODE_OFF))
			uart_puts("off\n");
		else
			protocol_commanderror();
		break;
	case PROTOCOL_COMMAND_SET:
		uart_puts("set\n");
		break;
	case PROTOCOL_COMMAND_INVALID:
		uart_puts("?\n");
		break;
	}
}

static protocol_command protocol_parsecommand() {
	switch (command[0]) {
	case 'O':
		switch (command[1]) {
		case 'N':
			if (command[2] == '\0')
				return PROTOCOL_COMMAND_ON;
			break;
		case 'F':
			if ((command[2] == 'F') && (command[3] == '\0'))
				return PROTOCOL_COMMAND_OFF;
			break;
		}
		break;
	case 'S':
		if ((command[1] == 'E') && (command[2] == 'T') && (command[3] == '\0'))
			return PROTOCOL_COMMAND_SET;
		break;
	}

	return PROTOCOL_COMMAND_INVALID;

}

static bool protocol_readpart() {
	bool last = false;
	uint8_t commandpos;
	bool issep, islineend;
	for (commandpos = 0; commandpos < (sizeof(command) - 1); commandpos++) {
		uart_getch(&command[commandpos]);
		issep = (command[commandpos] == ',');
		islineend = (command[commandpos] == '\n');
		if (issep || islineend) {
			if (islineend)
				last = true;
			break;
		}
	}
	command[commandpos] = '\0';

	return last;
}

void protocol_checkcommand() {
	static uint8_t lastsequence = 0;
	bool last = false;
	protocol_command cmd;
	if (sequence != lastsequence) {

		last = protocol_readpart();
		cmd = protocol_parsecommand();
		protocol_handlecommand(cmd);

		while (!last) {
			last = protocol_readpart();
			uart_puts(command);
			uart_putch(',');
		}
		lastsequence = sequence;
	}
}
