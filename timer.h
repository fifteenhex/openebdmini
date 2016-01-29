#pragma once

#include "stm8.h"

void timer_interrupthandler(void)
__interrupt(INTERRUPT_TIM3_UPDATEOVERFLOW);

void timer_start(void);

void timer_stop(void);

void timer_init(void);
