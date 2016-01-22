#pragma once

#include <stdint.h>

void split(uint16_t value, uint16_t* buffer, int digits);
uint16_t pack(uint16_t* buffer, int digits);
void setuppins(volatile uint8_t* ddr, volatile uint8_t* cr1, uint8_t bits);
