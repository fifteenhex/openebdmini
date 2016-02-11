#pragma once

typedef enum {
	PROTOCOL_COMMAND_INVALID,
	PROTOCOL_COMMAND_ON,
	PROTOCOL_COMMAND_OFF,
	PROTOCOL_COMMAND_SET
} protocol_command;

void protocol_onbooted(void);
void protocol_sendstate(void);
void protocol_checkcommand(void);
