#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define GPIO_EC_A (GPIO_Pin_14)
#define GPIO_EC_B (GPIO_Pin_4)
#define GPIO_EC_K (GPIO_Pin_5)

#define EC_A  GPIO_ReadInputDataBit(GPIOB,GPIO_EC_A)
#define EC_B  GPIO_ReadInputDataBit(GPIOB,GPIO_EC_B)
#define EC_K  GPIO_ReadInputDataBit(GPIOB,GPIO_EC_K)

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数					    
#endif
