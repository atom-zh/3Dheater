#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

enum KEY
{
    key_release,
    key_enter,
	key_left,
	key_right
};

#define GPIO_EC_A (GPIO_Pin_3)
#define GPIO_EC_B (GPIO_Pin_4)
#define GPIO_EC_K (GPIO_Pin_5)

#define EC_A  GPIO_ReadInputDataBit(GPIOB,GPIO_EC_A)
#define EC_B  GPIO_ReadInputDataBit(GPIOB,GPIO_EC_B)
#define EC_K  GPIO_ReadInputDataBit(GPIOB,GPIO_EC_K)

void KEY_Init(void);//IO初始化
void KEY_update(enum KEY Read_Key);
u8 KEY_Scan(void);  	//按键扫描函数					    
#endif
