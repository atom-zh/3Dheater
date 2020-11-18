#include "BSP_Cfg.h"
#include "lcd.h"
#include "stdlib.h"
#include "font.h"  	 
#include "delay.h"

/*串行接口,内部调压，外部电阻取消
   驱动IC是:ST7565R(or compatible)
*/

/*
#define data_bus	P1

sbit	lcm_cs1	=P3^4;		//片选,L有效
sbit	lcm_res	=P3^3;		//复位,L有效
sbit	lcm_a0	=P3^2;		//L:Control data,H:Display data
sbit	lcm_wr	=P3^1;		//写,L有效
sbit	lcm_rd	=P3^0;		//读,L有效

sbit key=P2^0;

#define unsigned char unsigned char 
#define uint unsigned int
#define ulong unsigned long
*/

//LCD的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
unsigned char  LCD_GRAM[128][8];	 

void LCD_Set_Pos(unsigned char x, unsigned char y) 
{
#if 1
	x=x+4;
	LCD_WR_Byte(0xb0+y,LCD_CMD);                  //设置页地址（0~7）
	LCD_WR_Byte(((x>>4)&0x0f)+0x10,LCD_CMD);      //设置显示位置—列低地址
	LCD_WR_Byte(x&0x0f,LCD_CMD);                  //设置显示位置—列高地址
#else
	LCD_WR_Byte(0xb0+y,LCD_CMD);
	LCD_WR_Byte(((x&0xf0)>>4)|0x10,LCD_CMD);
	LCD_WR_Byte((x&0x0f)|0x01,LCD_CMD);
#endif
}

//更新显存到LCD
void LCD_Refresh_Gram(void)
{
	unsigned char  i,n;		    
	for(i=0;i<8;i++)  
	{
    LCD_Set_Pos(0,i);
		for(n=0;n<128;n++)
		  LCD_WR_Byte(LCD_GRAM[n][i],LCD_DATA); 
	}
}

//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void LCD_WR_Byte(unsigned char  dat,unsigned char  cmd)
{		  
	if(cmd)
	  LCD_DC_Set(); //数据模式
	else 
	  LCD_DC_Clr();	//命令模式 
	
	LCD_CS_Clr();//片选
	
	//添加硬件读写SPI函数
	SPI2_ReadWriteByte(dat);

	LCD_CS_Set();
	LCD_DC_Set();   	  
}

//开启LCD显示
void LCD_Display_On(void)
{
	LCD_WR_Byte(0X8D,LCD_CMD);  //SET DCDC命令
	LCD_WR_Byte(0X14,LCD_CMD);  //DCDC ON
	LCD_WR_Byte(0XAF,LCD_CMD);  //DISPLAY ON
}

//关闭LCD显示
void LCD_Display_Off(void)
{
	LCD_WR_Byte(0X8D,LCD_CMD);  //SET DCDC命令
	LCD_WR_Byte(0X10,LCD_CMD);  //DCDC OFF
	LCD_WR_Byte(0XAE,LCD_CMD);  //DISPLAY OFF
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void LCD_Clear(void)  
{
	unsigned char  i,n;
	for(i=0;i<8;i++)for(n=0;n<128;n++)LCD_GRAM[n][i]=0x00;  
	LCD_Refresh_Gram();//更新显示
}

//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void LCD_DrawPoint(unsigned char  x,unsigned char  y,unsigned char  t)
{
	unsigned char  pos,bx,temp=0;
	LCD_Set_Pos(x, y/8);  
	if(x>128||y>64)
		return;//超出范围了.
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
	if(t)
		LCD_GRAM[x][pos]|=temp;
	else
	  LCD_GRAM[x][pos]&=~temp;	

  //设置显示位置—列高地址   
	LCD_WR_Byte(LCD_GRAM[x][pos],LCD_DATA); 
}

void LCD_DrawLine(unsigned char  x,unsigned char  y,unsigned char  dir,unsigned char  len)
{
	unsigned char  pos;

	if(dir)
	{
	  for(pos=0;pos<len;pos++)
    {
       LCD_DrawPoint(pos,y,1);
    }
	}else
	{
		for(pos=0;pos<len;pos++)
    {
       LCD_DrawPoint(x,pos,1);
    }
	}
}

//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void LCD_Fill(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2,unsigned char  dot)  
{  
	unsigned char  x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)LCD_DrawPoint(x,y,dot);
	}													    
	LCD_Refresh_Gram();//更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12 
void LCD_ShowChar(unsigned char  x,unsigned char  y,unsigned char  chr,unsigned char  size,unsigned char  mode)
{      			    
	unsigned char  temp,t,t1;
	unsigned char  y0=y;
	chr=chr-' ';                   //得到偏移后的值
  for(t=0;t<size;t++)
  {
		if(size==12)
			temp=asc2_1206[chr][t];    //调用1206字体
		else
			temp=asc2_1608[chr][t];		 //调用1608字体 

    for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
				LCD_DrawPoint(x,y,mode);
			else
				LCD_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}
  }
}

void LCD_ShowChinese(unsigned char  x,unsigned char  y,unsigned char  no)//显示中文
{
	unsigned char  temp,t,t1;
	unsigned char  y0=y;
	unsigned char  mode=1;

  for(t=0;t<32;t++)
  {
        temp= CHINESE[no*2][t];                      
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
				LCD_DrawPoint(x,y,mode);
			else
                LCD_DrawPoint(x,y,!mode);

			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
  }       
}

//m^n函数
unsigned int LCD_pow(unsigned char  m,unsigned char  n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void LCD_ShowNum(unsigned char  x,unsigned char  y,unsigned int num,unsigned char  len,unsigned char  size)
{         	
	unsigned char  t,temp;
	unsigned char  enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
}

//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void LCD_ShowString(unsigned char  x,unsigned char  y,const unsigned char  *p)
{
#define MAX_CHAR_POSX 127
#define MAX_CHAR_POSY 63         
    while(*p!='\0')
    {
        if(x>MAX_CHAR_POSX)
        {
					x=0;
					y+=16;
				}
        if(y>MAX_CHAR_POSY)
        {
				  y=x=0;
					LCD_Clear();
				}

        LCD_ShowChar(x,y,*p,12,1);
        x+=8;
        p++;
    }
}

/*LCD模块初始化*/
void LCD_Init()
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PC,D,G端口时钟

	GPIO_InitStructure.GPIO_Pin = LCD_DC|LCD_RES|LCD_BL;	 //A0脚接到LCD的RST脚 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //A0脚接到LCD的RST脚 
	
	LCD_RST_Set();
	delay_ms(100);
	LCD_RST_Clr();
	delay_ms(100);
	LCD_RST_Set(); 

	LCD_CS_Clr();
  delay_ms(20);        
	LCD_WR_Byte(0xe2,LCD_CMD);	 /*软复位*/
	delay_ms(50);
	LCD_WR_Byte(0x2c,LCD_CMD);  /*升压步聚1*/
	delay_ms(50);	
	LCD_WR_Byte(0x2e,LCD_CMD);  /*升压步聚2*/
	delay_ms(50);
	LCD_WR_Byte(0x2f,LCD_CMD);  /*升压步聚3*/
	delay_ms(50);
	LCD_WR_Byte(0x24,LCD_CMD);  /*粗调对比度，可设置范围0x20～0x27*/
	LCD_WR_Byte(0x81,LCD_CMD);  /*微调对比度*/
	LCD_WR_Byte(0x1a,LCD_CMD);  /*0x1a,微调对比度的值，可设置范围0x00～0x3f*/
	LCD_WR_Byte(0xa2,LCD_CMD);  /*1/9偏压比（bias）*/

	//LCD_WR_Byte(0xc8,LCD_CMD);  /*行扫描顺序：从上到下*/
	//LCD_WR_Byte(0xa0,LCD_CMD);  /*列扫描顺序：从左到右*/
	LCD_WR_Byte(0xc0,LCD_CMD);  /*行扫描顺序：从下到上*/
	LCD_WR_Byte(0xa1,LCD_CMD);  /*列扫描顺序：从右到左*/

	LCD_WR_Byte(0x40,LCD_CMD);  /*起始行：第一行开始*/
	LCD_WR_Byte(0xaf,LCD_CMD);  /*开显示*/
	LCD_CS_Set();
	
	
	LCD_BL_Clr(); //open black light
}


/*全屏清屏*/
void clear_screen(void)
{
	unsigned char i,j;
 	for(i=0;i<9;i++)
	{
		LCD_Set_Pos(0,i);
		for(j=0;j<132;j++)
		{
			LCD_WR_Byte(0x00,LCD_DATA);
		}
	}
}

//===显示测试画面：例如全显示，隔行显示，隔列显示，雪花显示=====
void test_display(unsigned char data1,unsigned char data2)
{
	int i,j;
	for(j=0;j<8;j++)
	{
		LCD_Set_Pos(0,j);
		for(i=0;i<128;i++)
		{
			LCD_WR_Byte(data1,LCD_DATA);   
			LCD_WR_Byte(data2,LCD_DATA); 
		}
	}          
}
#if 0
/*显示128x64点阵图像*/
void display_graphic_128x64(unsigned char page,unsigned char column,unsigned char *dp)
{
	int i,j;
	for(j=0;j<8;j++)
	{
		lcm_cs1=0;
		lcd_address(page+j,column);
		for(i=0;i<128;i++)
		{
			transfer_data(*dp);   
			dp++;
		}
	}          
}
#endif
/*
void main(void)
 { 
 	initial_lcd();
	while(1)
	{
	  
										//clear all dots
		test_display(0xff,0xff);					//全显示					
		DelayKey(1 , 5);

		clear_screen();
		DelayKey(1 , 5);

		test_display(0x55,0xaa);					//雪花1
		DelayKey(1 , 5);

  		test_display(0xFF,0x00);				 
		DelayKey(1 , 5);

//		test_display(0x00,0xFF);				 
//		waitkey();
//		
//		test_display(0x55,0x55);				 
//		waitkey();
		test_display(0xAA,0xAA);				 
		DelayKey(1 , 5);

		clear_screen();	//clear all dots		
		display_graphic_128x64(1,1,bmp_12864_1); 	//显示12864点阵图片
		DelayKey(1 , 5);
	}
}
*/

unsigned char bmp_12864_1[]={
0XFF,0XFF,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,
0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,
0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,
0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,
0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,
0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,
0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,
0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0X03,0XFF,0XFF,
0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0XC0,0XC0,0X60,0X70,0XFC,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X60,0X70,0X38,
0X1C,0X0C,0X0C,0X0C,0X1C,0XF8,0XF8,0XE0,0X00,0X00,0X00,0XE0,0XF0,0XF8,0X1C,0X0C,
0X0C,0X0C,0X1C,0XF8,0XF0,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XE0,0XF8,0X3C,
0X1C,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0XE0,0XF0,0XF8,0XFC,
0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X80,0XC0,0XE0,0X78,0X3E,0X0F,0X07,0X01,0X00,0X00,0X00,0XF1,0XFB,0X3F,0X1E,0X0C,
0X0C,0X0C,0X1E,0X3F,0XFB,0XF0,0X00,0X00,0X00,0XF8,0XFC,0XFF,0X0F,0X07,0X06,0X06,
0X06,0X0E,0XFC,0XFC,0XF0,0X00,0XC0,0XE0,0XF8,0XBC,0X8F,0X87,0X81,0X80,0XFF,0XFF,
0XFF,0X80,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X0F,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0C,0X0E,0X0F,
0X0F,0X0D,0X0C,0X0C,0X0C,0X0C,0X0C,0X0C,0X00,0X00,0X00,0X07,0X0F,0X0E,0X1C,0X18,
0X18,0X1C,0X1C,0X0E,0X0F,0X03,0X00,0X00,0X00,0X03,0X07,0X0F,0X0C,0X1C,0X18,0X18,
0X1C,0X0E,0X0F,0X07,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0X0F,
0X0F,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XF8,0XF8,0XF8,0X18,0X18,
0X18,0X18,0X18,0X98,0XF8,0XD8,0XD8,0X98,0X98,0X98,0X98,0X98,0X98,0X18,0X18,0XF8,
0XF8,0XF8,0X00,0X00,0X00,0X00,0X00,0X18,0X18,0X18,0XF8,0XF8,0X18,0X18,0X18,0XF8,
0XF8,0XF8,0X18,0X18,0X18,0X00,0X00,0X80,0XE0,0XF0,0X78,0X38,0X18,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X84,0XFC,0XFC,0X60,
0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X60,0X00,0X00,0X00,0X00,0X00,0X00,0XF0,
0XF0,0X10,0X10,0X90,0XF0,0XF0,0X10,0XC0,0XC0,0XC0,0XC0,0XC0,0XFC,0XFC,0XD8,0XC0,
0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0X00,0X88,
0X88,0X9E,0XCF,0XC7,0X7D,0XF9,0X31,0X79,0X7D,0XCF,0XC7,0XC3,0X80,0X80,0X00,0XFF,
0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X20,0X20,0X20,0XFF,0XFF,0X20,0X20,0X20,0XFF,
0XFF,0XFF,0X20,0X20,0X24,0X06,0X07,0X83,0XC1,0XE0,0X78,0X3C,0X1C,0X08,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0XFC,0XFC,0X0C,0X0C,0X0C,0X0C,0X0F,0X0F,0X0C,
0X0C,0X0C,0X0C,0X0C,0X0C,0XFC,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,
0XFF,0X00,0X08,0X3F,0XF7,0XE0,0X80,0X60,0XF0,0XF8,0XDF,0XC7,0X41,0X40,0XFC,0XFC,
0X44,0X40,0X40,0X40,0X40,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,0XFF,0X80,0X80,
0X81,0X81,0X80,0X98,0X98,0X99,0XB3,0XB3,0XB6,0XE6,0X80,0X80,0X81,0X81,0X80,0XFF,
0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X80,0XC0,0XFC,0X7F,0X0F,0X00,0X00,0X00,0XFF,
0XFF,0XFF,0X00,0X00,0X02,0X87,0X83,0XC3,0XE1,0X70,0X38,0X3C,0X1E,0X0C,0X04,0X00,
0X00,0X00,0X00,0X00,0XE0,0XF0,0X67,0X07,0X07,0X46,0XE6,0XE6,0X86,0X06,0X06,0X26,
0XE6,0XE6,0XC6,0X06,0X06,0X07,0X67,0XE0,0XC0,0X80,0X00,0X00,0X00,0X00,0X00,0XFF,
0XFF,0X00,0X08,0X18,0X1E,0X0F,0X07,0X18,0X18,0X18,0X18,0X18,0X18,0X18,0XFF,0XFF,
0X18,0X18,0X18,0X18,0X18,0X18,0X18,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XFF,
0XFF,0XFF,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC7,0XC7,0XC7,0XC1,0XC1,
0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC1,0XC7,
0XC7,0XC7,0XC0,0XC0,0XC0,0XC0,0XC2,0XC3,0XC3,0XC1,0XC0,0XC0,0XC0,0XC0,0XC0,0XC1,
0XC1,0XC1,0XC0,0XC1,0XC3,0XC3,0XC1,0XC1,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,
0XC0,0XC0,0XC2,0XC3,0XC3,0XC1,0XC0,0XC0,0XC0,0XC0,0XC1,0XC3,0XC3,0XC0,0XC0,0XC0,
0XC0,0XC3,0XC3,0XC3,0XC0,0XC0,0XC0,0XC1,0XC3,0XC3,0XC2,0XC0,0XC0,0XC0,0XC0,0XC3,
0XC3,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XCF,0XCF,
0XC4,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XC0,0XFF,0XFF,
};
