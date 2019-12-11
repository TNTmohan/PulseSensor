#include "ADC10.h"

void ADC_init() {
	ADC10AE0 |= BIT3;
	ADC10CTL1 |= INCH_3;
	ADC10CTL0 |= ADC10ON + ENC;
}

unsigned int ADC_get() {
	ADC10CTL0 |= ADC10SC;
	while((ADC10CTL0 & ADC10IFG) == 0);
	return ADC10MEM;
}
