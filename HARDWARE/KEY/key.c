#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_EC_A|GPIO_EC_B|GPIO_EC_K;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	    
 	return 0;// �ް�������
}
