#include "exti.h"
#include "key.h"
#include "delay.h"
#include "usart.h"

//外部中断0服务程序
void EXTIX_Init(void)
{
 
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	KEY_Init();	 //	按键端口初始化
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

#if 0
    //GPIOB.3	  中断线以及中断初始化配置  上升沿触发	//EC_A
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
#endif

    //GPIOB.4	  中断线以及中断初始化配置  上升沿触发	//EC_B
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
	

	//GPIOB.5	  中断线以及中断初始化配置 上升沿触发 EC_K
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5); 
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void EXTI4_IRQHandler(void)
{
    DEBUG("knob: trigger \r\n");
	delay_ms(1);
	if(EC_B==1)	
	{
        if(EC_A == 1)
        {
            DEBUG("knob: +1 \r\n");
            KEY_update(KEY_LEFT);
            delay_ms(10);
        }else{
            DEBUG("knob: -1 \r\n");
            KEY_update(KEY_RIGHT);
        }
	 }
 	 EXTI_ClearITPendingBit(EXTI_Line4);
}

//外部中断9_5服务程序 
void EXTI9_5_IRQHandler(void)
{
	DEBUG("knob: trigger \r\n");
	delay_ms(10);
	if(EC_K==1)	
	{
        DEBUG("key: press \r\n");
        KEY_update(KEY_ENTER);
        delay_ms(10);
	}else{
        KEY_update(KEY_RELEASE);
        DEBUG("key: release \r\n");
	}	
    EXTI_ClearITPendingBit(EXTI_Line5);
}
