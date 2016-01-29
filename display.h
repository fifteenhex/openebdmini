#pragma once

#include "stm8.h"

typedef enum {
	VOLTS, AMPS, AMPHOURS, WATTS, TIME, DISPMODE_END
} displaymode;

void display_init(void);
void display_update(void);

void display_refresh(void)
__interrupt(INTERRUPT_TIM4);
