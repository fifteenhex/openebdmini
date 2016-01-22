#include "state.h"

bool highgain = false;
uint16_t lvc = 2000;
uint16_t targetamps = 1234;
uint16_t volts = 0;
uint16_t amps = 0;
uint16_t amphours = 0;
uint16_t watts = 0;
uint16_t loadduty = 800;
uint8_t digitbeingset = 0;
displaymode dm = VOLTS;
operationmode om = OPMODE_OFF;
