#include "stm32f10x.h"
#include "BSP_Cfg.h"
#include "led.h"
#include "delay.h"
#include "exti.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "temp.h"
#include "bmp.h"
#include "weight.h"

void display_info(void)
{
	LCD_ShowString(1,1, "hum:   %|tmp:  C");
	LCD_DrawLine(0,0,1,128);
	LCD_DrawLine(0,13,1,128);
	LCD_DrawLine(0,63,1,128);
	
	LCD_DrawLine(127,0,0,64);
	LCD_DrawLine(0,0,0,64);
#if 1
  LCD_ShowString(1,14,"weight1:      g"); 
	LCD_ShowString(1,26,"weight2:      g"); 
  LCD_ShowChinese(1,47,0);
	LCD_ShowChinese(19,47,1);
	LCD_ShowChinese(37,47,2);
	LCD_ShowChinese(55,47,3);
#endif
}

void update_temp(void)
{
	  u16 adcx;
	  float temp = 25, weight = 0;
	
		adcx=Get_Temp_Average(ADC_Channel_1,10);
		//LCD_ShowxNum(156,130,adcx,4,16,0);//显示ADC的值
		temp=(float)adcx*(3.3/4096);
		//LCD_ShowxNum(156,150,adcx,1,16,0);//显示电压值
		// printf("The adc raw is %0.2f v\n", temp);
		temp-=adcx;
		temp*=1000;

		LCD_ShowNum(100,1,25,3,12);//显示ASCII数字 wendu
}

void update_hum(void)
{
  LCD_ShowNum(33,1,80,4,12);//显示ASCII数字 shidu
}

void update_weight1(void)
{
		long count = 0;
	  float weight = 0;
	
		count=HX711_Read()-120876; //加零点漂移
		weight=(float)count/700; 
    printf("Weight is raw: count=%ld, weight=%0.3f  g\n", count, weight);
	  LCD_ShowNum(64,14,weight,3,12);//显示ASCII数字
}

void update_weight2(void)
{
    LCD_ShowNum(64,26,444,3,12);//显示ASCII数字
}


int main(void)
{
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	SPI2_Init(); 			     //SPI 初始化 	

	EXTIX_Init();
 	LED_Init();
	Temp_Init();
  Init_Hx711();

  LCD_Init();			 //初始化LCD

	delay_ms(500);
	LCD_Clear();
	display_info();

  printf("Hello World!\r\n");
	while(1)
	{	
		delay_ms(500);
		update_temp();
		update_hum();
		update_weight1();
		update_weight2();
	}
}

