#include <msp430.h> 
#include "ADC10.h"//ADC10模块
#include "LED.h"//LED模块
#include "USARTA0.h"//串口通信模块
#include "my_string.h"//使用到的字符串处理函数
#include "nokia5110.h"//LCD模块


unsigned int heart_signal, heart_signal_buffer;//心跳信号，心跳信号缓存（上一次的心跳信号）
unsigned int heartbeat_flag, bluetooth_state;//心跳标志，蓝牙连接状态
unsigned int IBI, BPM;
unsigned int time_ms; //全局计时器（检测到心跳时清零）

char info_to_send[10], buffer[10], info_to_print[20];//蓝牙发送字符串、缓冲字符串、LCD显示字符串

void init();//系统初始化
void GPIO_init();//GPIO初始化
void send_signal();//蓝牙发送心跳信号
void send_bpm();//蓝牙发送BPM、IBI
void TA0_init();//定时器初始化
void print_bpm();//LCD显示BPM值

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    init();//系统初始化
    while(1) {
        bluetooth_state = (P1IN & BIT4);//检测蓝牙连接状态
        LED_blink();//检测是否有心跳发生，LED闪烁
        if(heartbeat_flag == 1) {//检测到有心跳发生
            IBI = time_ms % 1000;//两次心跳发生的间隔（IBI）计算, 单位:ms
            BPM = 6000 / IBI;//BPM计算，单位:次/min
            time_ms = 0;//清零全局计时器
            print_bpm();//LCD显示BPM值

            _DINT();//关中断总控位
            send_bpm();//蓝牙发送BPM、IBI的值
            _EINT();//开中断总控位
        }
        if(time_ms > 400) {
            //长时间无心跳信号产生，LCD显示信息
            LCD_write_english_string(0, 0, "Please put your finger on the sensor and wait for a while.");
            time_ms = 0;//全局计时器清零
        }
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_1ms() {//1ms中断
    time_ms = time_ms + 1;
    heart_signal_buffer = heart_signal;
    heart_signal = ADC_get();//将采集到的心跳信号转化为数字量
    send_signal();//蓝牙发送心跳信号signal
}

void init() {
    ADC_init();
    USARTA0_init();
    GPIO_init();
    TA0_init();
    LCD_init();

    //上电显示信息
    LCD_write_english_string(0, 0, 
        "Please put your finger on the sensor and wait for a while.");
}

void GPIO_init() {
    //LED P2.5
    P2SEL &= ~BIT5; P2SEL2 &= ~BIT5;
    P2DIR |= BIT5; P2OUT |= BIT5;
    //ADC10 P1.3
    P1SEL |= BIT3;
    //蓝牙连接状态 P1.4
    P1SEL &= ~BIT4; P1SEL2 &= ~BIT4;
    P1DIR &= ~BIT4;
}

void TA0_init() {
    BCSCTL1 = CALBC1_1MHZ; //初始化SMCLK为1MHz
    DCOCTL = CALDCO_1MHZ;
    BCSCTL2 |= DIVS_0;    //控制输出时钟不分频   
    //设置TA0定时器与TA捕捉/比较器0单元
    TA0CTL |= ID_0 + TACLR + TASSEL_2 + MC_1;  //设置TA控制寄存器的值
    TA0CCR0 = 10869;  //TA比较控制寄存器0值为10869
    TA0CCTL0 |= CCIE;  //比较相等时CCR0中断允许
    
    _EINT();  //允许中断
}

void send_signal() {
    if(bluetooth_state != 0) {
        //蓝牙已连接
        //发送signal,数据格式为"S" + str(signal)
        strcpy(info_to_send, "S");
        strcat(info_to_send, itoa(heart_signal, buffer));
        strcat(info_to_send, "\r\n");
        USARTA0_send(info_to_send);
    }
}

void send_bpm() {
    if(bluetooth_state != 0) {
        //蓝牙已连接

        //发送BPM,数据格式为"B" + str(BPM)
        strcpy(info_to_send, "B");
        strcat(info_to_send, itoa(BPM, buffer));
        strcat(info_to_send, "\r\n");
        USARTA0_send(info_to_send);

        //发送IBI,数据格式为"Q" + str(IBI)
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
        //LCD显示BPM,数据格式为"BPM： " + str(BPM)
        strcpy(info_to_print, "BPM: ");
        strcat(info_to_print, itoa(BPM, buffer));
        strcat(info_to_print, "          ");
        LCD_write_english_string(0, 1, info_to_print);
    } else {
        LCD_write_english_string(0, 1, "Please wait for a while...");
    }
}
