#include "state.h"
#include "adc.h"

#define SAMPLES 16

typedef struct {
	int channel;
	uint16_t* samples;
} channelsamples;

static uint16_t voltsamples[SAMPLES] = { 0 };
static uint16_t volthighgainsamples[SAMPLES] = { 0 };
static uint16_t shuntsamples[SAMPLES] = { 0 };

static channelsamples channels[] = { //
		{ .channel = ADC_VIN, .samples = voltsamples }, //
				{ .channel = ADC_VIN_HIGHGAIN, .samples = volthighgainsamples }, //
				{ .channel = ADC_SHUNT, .samples = shuntsamples } //
		};

static bool conversionfinished = false;

static inline void adc_clearint() {
	ADC_CSR &= ~ADC_CSR_EOC;
}

static inline void adc_interrupthandler_body(void) {
	static uint8_t channel = 0;
	static uint8_t sample = 0;

	channels[channel].samples[sample] = adc_readresult();
	channel++;
	if (channel == 3) {
		channel = 0;
		sample++;
		if (sample == SAMPLES) {
			sample = 0;
			adc_clearint();
			conversionfinished = true;
		}
	}

	if (!conversionfinished)
		adc_triggerconversion(channels[channel].channel);
}

void adc_interrupthandler(void)
__interrupt( INTERRUPT_ADC1) {
	adc_interrupthandler_body();
}

static inline void adc_setchan(int which) {
	uint8_t csr = ADC_CSR;
	csr &= ~(0b111 | ADC_CSR_EOC);
	csr |= which;
	ADC_CSR = csr;
}

static inline void adc_triggerconversion(int channel) {
	adc_setchan(channel);
	ADC_CR1 |= ADC_CR1_ADON;
}

static inline uint16_t adc_readresult() {
	uint16_t result = (((uint16_t) ADC_DRH) << 2) | ADC_DRL;
	return result;
}

static inline void adc_startconversion(void) {
	adc_triggerconversion(channels[0].channel);
}

void adc_init(void) {
	adc_setchan(ADC_VIN);
	ADC_CSR |= ADC_CSR_EOCIE;
	ADC_CR1 |= ADC_CR1_ADON;
	adc_startconversion();
}

bool adc_updatereadings(void) {
	unsigned i;
	uint32_t voltsum = 0;
	uint32_t shuntsum = 0;
	uint32_t volthighgainsum = 0;
	uint32_t wattstemp = 0;
	uint16_t lowgainvolts, highgainvolts;

	if (conversionfinished) {
		// trigger the next round
		conversionfinished = false;
		adc_startconversion();

		for (i = 0; i < SAMPLES; i++) {
			shuntsum += shuntsamples[i];
			voltsum += voltsamples[i];
			volthighgainsum += volthighgainsamples[i];
		}

		amps = ((shuntsum / SAMPLES) * MICROVOLTSPERSTEP_SHUNT) / 20;
		highgainvolts = (((volthighgainsum / SAMPLES)
				* MILLIVOLTSPERSTEP_HIGHGAIN) / 10) - HIGHOFFSET;
		lowgainvolts = (voltsum / SAMPLES) * (MILLIVOLTSPERSTEP);
		highgain = lowgainvolts <= 6000;
		if (highgain)
			volts = highgainvolts;
		else
			volts = lowgainvolts;

		wattstemp = ((uint32_t) amps * (uint32_t) volts) / 1000;
		watts = (uint16_t) wattstemp;

		return true;
	} else
		return false;

}
