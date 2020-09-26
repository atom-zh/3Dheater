#include "heating.h"
#include "delay.h"
#include "usart.h"

//加热接口初始化
void Heating_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_HEATING;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //PA0设置成输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA
}

void Heating_Ctrl(Option ctrl)
{
    if(ctrl)
    {
        GPIO_SetBits(GPIOA,GPIO_HEATING);
    }else{
        GPIO_ResetBits(GPIOA,GPIO_HEATING);
    }
}
