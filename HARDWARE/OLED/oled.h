#ifndef __OLED_H
#define __OLED_H 

#include "stm32f10x.h"

/*
GND: 电源地
VCC: 电源正 3-5V
D0: SPI时钟
D1: SPI数据  MOSI
RES: 复位
DC: 数据命令切换
CS: 片选，可接地
*/
//初始化SSD1306	
#define OLED_DC		GPIO_Pin_10
#define OLED_RES	GPIO_Pin_11
#define OLED_CS		GPIO_Pin_12

//-----------------OLED端口定义----------------
#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,OLED_DC) //DC
#define OLED_DC_Set() GPIO_SetBits(GPIOB,OLED_DC)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,OLED_RES) //RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,OLED_RES)

#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,OLED_CS) //CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,OLED_CS)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_WR_Byte(unsigned char  dat,unsigned char  cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   

void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(unsigned char  x,unsigned char  y,unsigned char  t);
void OLED_Fill(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2,unsigned char  dot);
void OLED_ShowChinese(unsigned char  x,unsigned char  y,unsigned char  no);
void OLED_ShowChar(unsigned char  x,unsigned char  y,unsigned char  chr,unsigned char  size,unsigned char  mode);
void OLED_ShowNum(unsigned char  x,unsigned char  y,unsigned int num,unsigned char  len,unsigned char  size);
void OLED_ShowString(unsigned char  x,unsigned char  y,const unsigned char  *p);
void OLED_Set_Pos(unsigned char x, unsigned char y);
#endif
