#include <msp430.h> 
#include "ADC10.h"
#include "LED.h"

/**
 * main.c
 */
unsigned int heart_signal, heart_signal_buffer;
unsigned int heartbeat_count;
unsigned int heartbeat_flag;

void init();

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1SEL |= BIT3;
	P2SEL &= ~BIT0; P2SEL &= ~BIT0; P2DIR |= BIT0; P2OUT |= BIT0;
	init();
	while(1) {
		heart_signal_buffer = heart_signal;
        heart_signal = ADC_get();
        LED_blink();
	}
	return 0;
}

void init() {
    ADC_init();
    LED_init();
}
