#include "BSP_Cfg.h"
#include "lcd.h"
#include "stdlib.h"
#include "font.h"  	 
#include "delay.h"

/*���нӿ�,�ڲ���ѹ���ⲿ����ȡ��
   ����IC��:ST7565R(or compatible)
*/

/*
#define data_bus	P1

sbit	lcm_cs1	=P3^4;		//Ƭѡ,L��Ч
sbit	lcm_res	=P3^3;		//��λ,L��Ч
sbit	lcm_a0	=P3^2;		//L:Control data,H:Display data
sbit	lcm_wr	=P3^1;		//д,L��Ч
sbit	lcm_rd	=P3^0;		//��,L��Ч

sbit key=P2^0;

#define unsigned char unsigned char 
#define uint unsigned int
#define ulong unsigned long
*/

//LCD���Դ�
//��Ÿ�ʽ����.
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
	LCD_WR_Byte(0xb0+y,LCD_CMD);                  //����ҳ��ַ��0~7��
	LCD_WR_Byte(((x>>4)&0x0f)+0x10,LCD_CMD);      //������ʾλ�á��е͵�ַ
	LCD_WR_Byte(x&0x0f,LCD_CMD);                  //������ʾλ�á��иߵ�ַ
#else
	LCD_WR_Byte(0xb0+y,LCD_CMD);
	LCD_WR_Byte(((x&0xf0)>>4)|0x10,LCD_CMD);
	LCD_WR_Byte((x&0x0f)|0x01,LCD_CMD);
#endif
}

//�����Դ浽LCD
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

//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void LCD_WR_Byte(unsigned char  dat,unsigned char  cmd)
{		  
	if(cmd)
	  LCD_DC_Set(); //����ģʽ
	else 
	  LCD_DC_Clr();	//����ģʽ 
	
	LCD_CS_Clr();//Ƭѡ
	
	//���Ӳ����дSPI����
	SPI2_ReadWriteByte(dat);

	LCD_CS_Set();
	LCD_DC_Set();   	  
}

//����LCD��ʾ
void LCD_Display_On(void)
{
	LCD_WR_Byte(0X8D,LCD_CMD);  //SET DCDC����
	LCD_WR_Byte(0X14,LCD_CMD);  //DCDC ON
	LCD_WR_Byte(0XAF,LCD_CMD);  //DISPLAY ON
}

//�ر�LCD��ʾ
void LCD_Display_Off(void)
{
	LCD_WR_Byte(0X8D,LCD_CMD);  //SET DCDC����
	LCD_WR_Byte(0X10,LCD_CMD);  //DCDC OFF
	LCD_WR_Byte(0XAE,LCD_CMD);  //DISPLAY OFF
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void LCD_Clear(void)  
{
	unsigned char  i,n;
	for(i=0;i<8;i++)for(n=0;n<128;n++)LCD_GRAM[n][i]=0x00;  
	LCD_Refresh_Gram();//������ʾ
}

//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void LCD_DrawPoint(unsigned char  x,unsigned char  y,unsigned char  t)
{
	unsigned char  pos,bx,temp=0;
	LCD_Set_Pos(x, y/8);  
	if(x>128||y>64)
		return;//������Χ��.
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
	if(t)
		LCD_GRAM[x][pos]|=temp;
	else
	  LCD_GRAM[x][pos]&=~temp;	

  //������ʾλ�á��иߵ�ַ   
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

//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void LCD_Fill(unsigned char  x1,unsigned char  y1,unsigned char  x2,unsigned char  y2,unsigned char  dot)  
{  
	unsigned char  x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)LCD_DrawPoint(x,y,dot);
	}													    
	LCD_Refresh_Gram();//������ʾ
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ������ 16/12 
void LCD_ShowChar(unsigned char  x,unsigned char  y,unsigned char  chr,unsigned char  size,unsigned char  mode)
{      			    
	unsigned char  temp,t,t1;
	unsigned char  y0=y;
	chr=chr-' ';                   //�õ�ƫ�ƺ��ֵ
  for(t=0;t<size;t++)
  {
		if(size==12)
			temp=asc2_1206[chr][t];    //����1206����
		else
			temp=asc2_1608[chr][t];		 //����1608���� 

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

void LCD_ShowChinese(unsigned char  x,unsigned char  y,unsigned char  no)//��ʾ����
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

//m^n����
unsigned int LCD_pow(unsigned char  m,unsigned char  n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}

//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
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

//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
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

/*LCDģ���ʼ��*/
void LCD_Init()
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PC,D,G�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = LCD_DC|LCD_RES|LCD_BL;	 //A0�Žӵ�LCD��RST�� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //A0�Žӵ�LCD��RST�� 
	
	LCD_RST_Set();
	delay_ms(100);
	LCD_RST_Clr();
	delay_ms(100);
	LCD_RST_Set(); 

	LCD_CS_Clr();
  delay_ms(20);        
	LCD_WR_Byte(0xe2,LCD_CMD);	 /*��λ*/
	delay_ms(50);
	LCD_WR_Byte(0x2c,LCD_CMD);  /*��ѹ����1*/
	delay_ms(50);	
	LCD_WR_Byte(0x2e,LCD_CMD);  /*��ѹ����2*/
	delay_ms(50);
	LCD_WR_Byte(0x2f,LCD_CMD);  /*��ѹ����3*/
	delay_ms(50);
	LCD_WR_Byte(0x24,LCD_CMD);  /*�ֵ��Աȶȣ������÷�Χ0x20��0x27*/
	LCD_WR_Byte(0x81,LCD_CMD);  /*΢���Աȶ�*/
	LCD_WR_Byte(0x1a,LCD_CMD);  /*0x1a,΢���Աȶȵ�ֵ�������÷�Χ0x00��0x3f*/
	LCD_WR_Byte(0xa2,LCD_CMD);  /*1/9ƫѹ�ȣ�bias��*/

	//LCD_WR_Byte(0xc8,LCD_CMD);  /*��ɨ��˳�򣺴��ϵ���*/
	//LCD_WR_Byte(0xa0,LCD_CMD);  /*��ɨ��˳�򣺴�����*/
	LCD_WR_Byte(0xc0,LCD_CMD);  /*��ɨ��˳�򣺴��µ���*/
	LCD_WR_Byte(0xa1,LCD_CMD);  /*��ɨ��˳�򣺴��ҵ���*/

	LCD_WR_Byte(0x40,LCD_CMD);  /*��ʼ�У���һ�п�ʼ*/
	LCD_WR_Byte(0xaf,LCD_CMD);  /*����ʾ*/
	LCD_CS_Set();
	
	
	LCD_BL_Clr(); //open black light
}


/*ȫ������*/
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

//===��ʾ���Ի��棺����ȫ��ʾ��������ʾ��������ʾ��ѩ����ʾ=====
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
/*��ʾ128x64����ͼ��*/
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
		test_display(0xff,0xff);					//ȫ��ʾ					
		DelayKey(1 , 5);

		clear_screen();
		DelayKey(1 , 5);

		test_display(0x55,0xaa);					//ѩ��1
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
		display_graphic_128x64(1,1,bmp_12864_1); 	//��ʾ12864����ͼƬ
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
