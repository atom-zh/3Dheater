#include "heating.h"
#include "delay.h"
#include "usart.h"

//���Ƚӿڳ�ʼ��
void Heating_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_HEATING;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //PA0���ó����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA
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
