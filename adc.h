#pragma once

#include <stdbool.h>

#include "stm8.h"

#define ADC_VIN 4
#define ADC_SHUNT 3
#define ADC_VIN_HIGHGAIN 5

#define MILLIVOLTSPERSTEP 20

// tenths of millivolts
#define MILLIVOLTSPERSTEP_HIGHGAIN 65
#define HIGHOFFSET 20

#define MICROVOLTSPERSTEP_SHUNT 68

void adc_interrupthandler(void)
__interrupt( INTERRUPT_ADC1);

uint16_t readadc(int which);
bool adc_updatereadings(void);
void adc_init(void);

static inline void adc_setchan(int which);
static inline uint16_t adc_readresult(void);
static inline void adc_triggerconversion(int channel);
