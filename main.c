#include <msp430.h> 
#include "ADC10.h"
#include "LED.h"
#include "USARTA0.h"
#include "my_string.h"
#include "nokia5110.h"

/**
 * main.c
 */

unsigned int heart_signal, heart_signal_buffer;
unsigned int heartbeat_flag, bluetooth_state;
unsigned int IBI, BPM;
unsigned int time_ms;

char info_to_send[10], buffer[10], info_to_print[20];
void init();
void GPIO_init();
void send_signal();
void send_bpm();
void TA0_init();
void print_bpm();

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    init();
    while(1) {
        bluetooth_state = (P1IN & BIT4);
        LED_blink();
        if(heartbeat_flag == 1) {
            IBI = time_ms % 1000;
            BPM = 6000 / IBI;
            time_ms = 0;
            print_bpm();
            _DINT();
            send_bpm();
            _EINT();
        }
        if(time_ms > 400) {
            LCD_write_english_string(0, 0, "Please put your finger on the sensor and wait for a while.");
            time_ms = 0;
        }
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_1ms() {
    time_ms = time_ms + 1;
    heart_signal_buffer = heart_signal;
    heart_signal = ADC_get();
    send_signal();
}

void init() {
    ADC_init();
    USARTA0_init();
    GPIO_init();
    TA0_init();
    LCD_init();
    LCD_write_english_string(0, 0, "Please put your finger on the sensor and wait for a while.");
}

void GPIO_init() {
    //LED P2.4
    P2SEL &= ~BIT5; P2SEL2 &= ~BIT5;
    P2DIR |= BIT5; P2OUT |= BIT5;
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

void send_signal() {
    if(bluetooth_state != 0) {
        strcpy(info_to_send, "S");
        char buffer[5];
        strcat(info_to_send, itoa(heart_signal, buffer));
        strcat(info_to_send, "\r\n");
        USARTA0_send(info_to_send);
    }
}
void send_bpm() {
    if(bluetooth_state != 0) {
        strcpy(info_to_send, "B");
        strcat(info_to_send, itoa(BPM, buffer));
        strcat(info_to_send, "\r\n");
        USARTA0_send(info_to_send);

        strcpy(info_to_send, "Q");
        strcat(info_to_send, itoa(IBI, buffer));
        strcat(info_to_send, "\r\n");
        USARTA0_send(info_to_send);
    }
}

void print_bpm() {
    LCD_clear();
    //LCD_write_english_string(5, 0, "         ");
    //LCD_write_english_string(5, 1, "        ");
    if((BPM <= 160) && (BPM >= 40)) {
        strcpy(info_to_print, "BPM: ");
        strcat(info_to_print, itoa(BPM, buffer));
        strcat(info_to_print, "          ");
        LCD_write_english_string(0, 1, info_to_print);
    } else {
        LCD_write_english_string(0, 1, "Please wait for a while...");
    }
}
