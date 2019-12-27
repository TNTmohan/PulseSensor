#include "LED.h"

extern unsigned int heart_signal, heart_signal_buffer;
extern unsigned int heartbeat_flag;


void LED_blink() {
	if((heart_signal >= THRESHOLD) && (heart_signal_buffer <= THRESHOLD)) {
		//检测到心跳
		blink();
		heartbeat_flag = 1;
	} else {
		heartbeat_flag = 0;
	}
 }

//LED闪烁函数
void blink() {
	P2OUT &= ~BIT5;
	delay_ms(1);
	P2OUT |= BIT5;
}

//1ms delay函数
void delay_ms(unsigned int time_ms) {
    while(time_ms--)
	__delay_cycles(1087);
}
