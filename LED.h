#ifndef __LED_h
#define __LED_h
#include "msp430.h"
#define THRESHOLD 600 //心跳信号阈值，大于高阈值判断为心跳发生

void LED_blink();
void blink();
void delay_ms(unsigned int time_ms);

#endif
 
