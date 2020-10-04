#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

typedef enum
{
    KEY_RELEASE,
    KEY_ENTER,
    KEY_LEFT,
    KEY_RIGHT
}KEY;

#define GPIO_EC_A (GPIO_Pin_3)
#define GPIO_EC_B (GPIO_Pin_4)
#define GPIO_EC_K (GPIO_Pin_5)

#define EC_A  GPIO_ReadInputDataBit(GPIOB,GPIO_EC_A)
#define EC_B  GPIO_ReadInputDataBit(GPIOB,GPIO_EC_B)
#define EC_K  GPIO_ReadInputDataBit(GPIOB,GPIO_EC_K)

void KEY_Init(void);//IO��ʼ��
void KEY_update(KEY Read_Key);
u8 KEY_Scan(void);  	//����ɨ�躯��					    
#endif
