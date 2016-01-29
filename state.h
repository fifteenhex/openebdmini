#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "display.h"

typedef enum {
	OPMODE_OFF, // load is turned off
	OPMODE_SET, // user is setting parameters, load is off
	OPMODE_ON, // load is on
	OPMODE_LVC, // low voltage cutoff triggered, load is off
} operationmode;

extern bool highgain;
extern uint16_t lvc;
extern uint16_t targetamps;
extern uint16_t volts;
extern uint16_t amps;
extern uint16_t amphours;
extern uint16_t watts;
extern uint16_t loadduty;
extern uint8_t digitbeingset;
extern uint16_t time;
extern displaymode dm;
extern operationmode om;
