
#ifndef __nokia_5110_h_
#define __nokia_5110_h_
#include "msp430G2553.h"
#define LCD_5110_DIR  P2DIR
#define LCD_5110_OUT  P2OUT
#define   LCD_RST    4
#define   LCD_SCE    3
#define   LCD_DC     2
#define   LCD_DN     1
#define   LCD_SCLK   0
void LCD_init(void);
void LCD_clear(void);
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
void LCD_write_char(unsigned char c);
void LCD_write_byte(unsigned char dat, unsigned char dc);             
#endif
