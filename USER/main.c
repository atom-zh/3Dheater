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
#include "humidity.h"
#include "fan.h"
#include "heating.h"
#include "menu.h"

struct Dev_Info dev;

void display_init(void)
{
    LCD_ShowString(1,1, (const unsigned char *)"hum:   %|tmp:  C");
    LCD_DrawLine(0,0,1,128);
    LCD_DrawLine(0,13,1,128);
    LCD_DrawLine(0,63,1,128);
	
    LCD_DrawLine(127,0,0,64);
    LCD_DrawLine(0,0,0,64);
#if 1
    LCD_ShowString(1,14,(const unsigned char *)"weight1:      g"); 
    LCD_ShowString(1,26,(const unsigned char *)"weight2:      g"); 
    LCD_ShowChinese(1,47,0);
    LCD_ShowChinese(19,47,1);
    LCD_ShowChinese(37,47,2);
    LCD_ShowChinese(55,47,3);
    LCD_ShowChinese(73,47,4);
    LCD_ShowChinese(91,47,5);
#endif
}

void update_temp(void)
{
    float temp = 25;
	
    temp=Get_Temp();
    LCD_ShowNum(100,1,temp,3,12);//显示ASCII数字 wendu
    dev.temp = temp;
}

void update_hum(void)
{
    unsigned int hum = 60;
    hum = ReadShtc3();
    LCD_ShowNum(33,1,hum,3,12);//显示ASCII数字 shidu
    dev.hum = hum;
}

void update_weight1(void)
{
    long count1 = 0;
    float weight1 = 0;
	
    count1=HX711_W1Read()-150000; //加零点漂移
    weight1=(float)count1/700; 
    printf("Weight1 is raw: count=%ld, weight=%0.3f  g\n", count1, weight1);
    LCD_ShowNum(64,14,weight1,3,12);//显示ASCII数字
}

void update_weight2(void)
{
    long count2 = 0;
    float weight2 = 0;
	
    count2=HX711_W2Read()-150000; //加零点漂移
    weight2=(float)count2/700; 
    printf("Weight2 is raw: count=%ld, weight=%0.3f  g\n", count2, weight2);
    LCD_ShowNum(64,26,weight2,3,12);//显示ASCII数字
}

void update_info(void)
{
    update_temp();
    update_hum();
    update_weight1();
    update_weight2();
}

int main(void)
{
    dev.status=MENU_INFO;
    dev.hum_th = 60;
    dev.temp_th = 20;

    delay_init();	    	 //延时函数初始化	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);	 	 //串口初始化为115200
    SPI2_Init(); 			 //SPI 初始化 	

    EXTIX_Init();
    //LED_Init();
    Temp_Init();
    humidity_init();
    Init_Hx711();

    LCD_Init();			 //初始化LCD

    delay_ms(500);
    LCD_Clear();
    display_init();
    
    Fan_Init();
    Heating_Init();
    
    printf("Hello World!\r\n");
    while(1)
    {
        delay_ms(100);
        dev.key_val=KEY_Scan();
        //printf("KEY SCAN: val %d\r\n", key_val);
        KEY_update(KEY_RELEASE);
        
        if(dev.key_val == KEY_ENTER && dev.status == MENU_INFO)
        {
            dev.status = MENU_ENTRY;
        }

        switch(dev.status)
        {
            case MENU_INFO:
            {
                update_info();
                delay_ms(1000);
                break;
            }
            case MENU_ENTRY:
            {
                LCD_Clear();
                delay_ms(100);
                menu_update(&dev);
                dev.status = MENU_CTRL;
                break;
            }
            case MENU_CTRL:
            {
                if(menu_update(&dev))
                    dev.status = MENU_EXIT;
                break;
            }
            case MENU_EXIT:
            {
                LCD_Clear();
                delay_ms(100);
                display_init();
                dev.status = MENU_INFO;
                break;
            }
            
            default: break;
        }
        
        if(dev.temp > 34)
            Fan_Ctrl(open);
        else
            Fan_Ctrl(close);

        if(dev.hum > 70)
            Heating_Ctrl(open);
        else
            Heating_Ctrl(close);
    }
}

