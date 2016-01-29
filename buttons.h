#pragma once

#include <stdbool.h>
#include "stm8.h"

typedef enum {
	PT_NONE, PT_DOWN, PT_SHORT, PT_LONG
} presstype;

void buttons_set_handler(void)
__interrupt( INTERRUPT_TLI);

void buttons_on_handler(void)
__interrupt(INTERRUPT_EXTI3);

void buttons_init(void);
bool buttons_check(void);
