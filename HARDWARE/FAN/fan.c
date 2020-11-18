#include "fan.h"
#include "delay.h"
#include "usart.h"

//风扇接口初始化
void Fan_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
 	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_FAN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Fan_Ctrl(Option ctrl)
{
    if(ctrl){
        GPIO_SetBits(GPIOA,GPIO_FAN);
        //printf("Fan_Ctrl: GPIO_SetBits\r\n");
    }else{
        GPIO_ResetBits(GPIOA,GPIO_FAN);
        //printf("Fan_Ctrl: GPIO_ResetBits\r\n");
    }
}
