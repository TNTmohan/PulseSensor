#ifndef __LED_h
#define __LED_h
#include "msp430.h"
#define THRESHOLD 700

void LED_init();
void LED_blink();
void blink();
void delay_ms(unsigned int time_ms);

#endif
