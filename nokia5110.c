

#include "nokia5110.h"
#include "english_6x8_pixel.h" //写英文头文件

/*---------------------------------------------------------------------
LCD_init          : 5110LCD初始化
---------------------------------------------------------------------*/
void LCD_init(void)
  {
  // 产生一个让LCD复位的低电平脉冲
 
    LCD_5110_DIR |= (0x01 << LCD_RST) + (0x01 << LCD_SCE) + (0x01 << LCD_DC) 
      + (0x01 << LCD_DN) + (0x01<< LCD_SCLK);
 
    LCD_5110_OUT &= ~(0x01 << LCD_RST);
   
    // LCD_RST = 1;
    LCD_5110_OUT |= (0x01 << LCD_RST);
    
    // 关闭LCD
  
    LCD_5110_OUT &= ~(0x01 << LCD_SCE);
   
    // 使能LCD
    //LCD_SCE = 1;
    LCD_5110_OUT |= (0x01 << LCD_SCE);
 
    LCD_write_byte(0x21, 0); // 使用扩展命令设置LCD模式
    LCD_write_byte(0xc8, 0); // 设置偏置电压
    LCD_write_byte(0x06, 0); // 温度校正
    LCD_write_byte(0x13, 0); // 1:48
    LCD_write_byte(0x20, 0); // 使用基本命令
    LCD_clear();         // 清屏
    LCD_write_byte(0x0c, 0); // 设定显示模式，正常显示
        
    // 关闭LCD
    LCD_5110_OUT &= ~(0x01 << LCD_SCE);
  }

/*---------------------------------------------------------------------
LCD_clear         : LCD清屏函数
---------------------------------------------------------------------*/
void LCD_clear(void)
  {
    unsigned int i;
    LCD_write_byte(0x0c, 0);   
    LCD_write_byte(0x80, 0);   
    for (i=0; i<504; i++)
      LCD_write_byte(0, 1);   
  }

/*---------------------------------------------------------------------
LCD_set_XY        : 设置LCD坐标函数
输入参数：X       ：0－83
          Y       ：0－5
---------------------------------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
  {
    LCD_write_byte(0x40 | Y, 0);  // column
    LCD_write_byte(0x80 | X, 0);  // row
  }

/*---------------------------------------------------------------------
LCD_write_char    : 显示英文字符
输入参数：c       ：显示的字符；
---------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
  {
    unsigned char line;
    c -= 32;
    for (line=0; line<6; line++)
      LCD_write_byte(font6x8[c][line], 1);
  }

/*---------------------------------------------------------------------
LCD_write_english_String  : 英文字符串显示函数
输入参数：*s      ：英文字符串指针；
          X、Y    : 显示字符串的位置,x 0-83 ,y 0-5
---------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
  {
    LCD_set_XY(X,Y);
    while (*s) 
    {
     LCD_write_char(*s);
     s++;
    }
  }

/*---------------------------------------------------------------------
LCD_write_byte    : 使用SPI接口写数据到LCD
输入参数：data    ：写入的数据；
          command ：写数据/命令选择；
---------------------------------------------------------------------*/
void LCD_write_byte(unsigned char data, unsigned char command)
  {
    unsigned char i;
    LCD_5110_OUT &= ~(0x01 << LCD_SCE); // msp430
    
    if (command == 0)                               // 传送命令
        LCD_5110_OUT &= ~(0x01 << LCD_DC);
    else                                            // 传送数据
  LCD_5110_OUT |= (0x01 << LCD_DC);
  for(i=0;i<8;i++)
  {
   if(data&0x80)
    LCD_5110_OUT |= (0x01 << LCD_DN);
   else
      LCD_5110_OUT &= ~(0x01 << LCD_DN);
   //SCLK = 0;
   LCD_5110_OUT &= ~(0x01 << LCD_SCLK);
   data = data << 1;
   LCD_5110_OUT |= (0x01 << LCD_SCLK);
  }
    LCD_5110_OUT |= (0x01 << LCD_SCE);
  }
