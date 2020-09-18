#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h"

/* OLED Pin*/
/*
GND: 电源地
VCC: 电源正 3-5V
D0: SPI时钟
D1: SPI数据  MOSI
RES: 复位
DC: 数据命令切换
CS: 片选，可接地
*/

/* LCD Pin 12864-14*/
/*
Pin No   |   Symbol    |   Level    |   Function
  7           DB6           H/L          SCK
  8           DB7           H/L          SI
  9           VDD           3.3V         电源正
  10          VSS           0V           电源负
  11          *LED+         3.3V         背光电压正极，3.3v
  12          /CS           L            片选信号
  13          /RES          L            复位脚，低电平有效
  14          A0(D/I)       H/L          命令数据切换，0：命令，1：数据
*/

//初始化SSD1306	
#define LCD_BL		GPIO_Pin_9
#define LCD_DC		GPIO_Pin_10
#define LCD_RES	  GPIO_Pin_11
#define LCD_CS		GPIO_Pin_12

//-----------------OLED端口定义----------------
#define LCD_BL_Clr() GPIO_ResetBits(GPIOB,LCD_BL) //DC
#define LCD_BL_Set() GPIO_SetBits(GPIOB,LCD_BL)

#define LCD_DC_Clr() GPIO_ResetBits(GPIOB,LCD_DC) //DC
#define LCD_DC_Set() GPIO_SetBits(GPIOB,LCD_DC)

#define LCD_RST_Clr() GPIO_ResetBits(GPIOB,LCD_RES) //RES
#define LCD_RST_Set() GPIO_SetBits(GPIOB,LCD_RES)

#define LCD_CS_Clr()  GPIO_ResetBits(GPIOB,LCD_CS) //CS
#define LCD_CS_Set()  GPIO_SetBits(GPIOB,LCD_CS)

#define LCD_CMD  0	//写命令
#define LCD_DATA 1	//写数据

//OLED控制用函数
void LCD_WR_Byte(unsigned char  dat,unsigned char  cmd);	    
void LCD_Display_On(void);
void LCD_Display_Off(void);
void LCD_Refresh_Gram(void);		   

void LCD_Init(void);
void LCD_Clear(void);
void LCD_DrawLine(unsigned char  x,unsigned char  y,unsigned char  dir,unsigned char  len);
void LCD_DrawPoint(unsigned char  x,unsigned char  y,unsigned char  t);
void LCD_Fill(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2,unsigned char  dot);
void LCD_ShowChinese(unsigned char  x,unsigned char  y,unsigned char  no);
void LCD_ShowChar(unsigned char  x,unsigned char  y,unsigned char  chr,unsigned char  size,unsigned char  mode);
void LCD_ShowNum(unsigned char  x,unsigned char  y,unsigned int num,unsigned char  len,unsigned char  size);
void LCD_ShowString(unsigned char  x,unsigned char  y,const unsigned char  *p);	 
void LCD_Set_Pos(unsigned char  x, unsigned char  y);

void clear_screen();
void test_display(unsigned char data1,unsigned char data2);
#endif  
	 
	 



