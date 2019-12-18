#include <msp430.h> 
#include "ADC10.h"
#include "LED.h"
#include "USARTA0.h"
#include "my_string.h"

/**
 * main.c
 */

unsigned int heart_signal, heart_signal_buffer;
unsigned int heartbeat_flag, bluetooth_state;
unsigned int IBI, BPM;
unsigned int time_ms;

char info_to_send[10] = "\0";

void init();
void GPIO_init();
void bluetooth_send();
void TA0_init();

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	init();
	while(1) {
	    bluetooth_state = (P1IN & BIT4);
        LED_blink();
        if(heartbeat_flag == 1) {
        	IBI = time_ms;
        	BPM = 6000 / IBI;
        	time_ms = 0;
        }
        if(bluetooth_state != 0) 
        	bluetooth_send();
	}
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_1ms() {
	time_ms = (time_ms + 1) % 1000;
	heart_signal_buffer = heart_signal;
    heart_signal = ADC_get();
}

void init() {
    ADC_init();
    USARTA0_init();
    GPIO_init();
    TA0_init();
}

void GPIO_init() {
	//LED P2.4
	P2SEL &= ~BIT4; P2SEL2 &= ~BIT4;
	P2DIR |= BIT4; P2OUT |= BIT4;
	//ADC10 P1.3
	P1SEL |= BIT3;
	//BLUETOOTH STATE P1.4
	P1SEL &= ~BIT4; P1SEL2 &= ~BIT4;
	P1DIR &= ~BIT4;
}

void TA0_init() {
	BCSCTL1 = CALBC1_1MHZ; //初始化SMCLK为1MHz 
    DCOCTL = CALDCO_1MHZ;
    BCSCTL2 |= DIVS_0;    //控制输出时钟不分频   
    //设置TA0定时器与TA捕捉/比较器0单元
    TA0CTL |= ID_0 + TACLR + TASSEL_2 + MC_1;  //设置TA控制寄存器的值
    TA0CCR0 = 10869;  //TA比较控制寄存器0值为1086
    TA0CCTL0 |= CCIE;  //比较相等时CCR0中断允许
    
    _EINT();  //允许中断
}

void bluetooth_send() {
	strcpy(info_to_send, "S");
    char tmp[5];
    strcat(info_to_send, itoa(heart_signal, tmp));
    strcat(info_to_send, "\r\n");
	USARTA0_send(info_to_send);
	if(heartbeat_flag == 1) {
		strcpy(info_to_send, "B");
		strcat(info_to_send, itoa(BPM, tmp));
    	strcat(info_to_send, "\r\n");
    	USARTA0_send(info_to_send);

    	strcpy(info_to_send, "Q");
		strcat(info_to_send, itoa(IBI, tmp));
    	strcat(info_to_send, "\r\n");
    	USARTA0_send(info_to_send);
	}
}
