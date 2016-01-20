#pragma once

typedef enum {
	VOLTS, AMPS, AMPHOURS, WATTS, DISPMODE_END
} displaymode;

void display_init(void);
void display_update(void);
