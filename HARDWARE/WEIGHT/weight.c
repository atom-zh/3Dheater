#include "weight.h"
#include "delay.h"

#define HX711_SCK		GPIO_Pin_0
#define HX711_DOUT	GPIO_Pin_1

//****************************************************
//初始化HX711
//****************************************************
void Init_Hx711(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
  //SCK
  GPIO_InitStructure.GPIO_Pin = HX711_SCK ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  //DOUT
  GPIO_InitStructure.GPIO_Pin = HX711_DOUT ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//****************************************************
//读取 HX711
//****************************************************
unsigned long HX711_Read(void)
{
	unsigned long val = 0; 
	unsigned char i = 0; 
	unsigned char a = 0;

	//while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6));
	//delay_ms(10);
  //printf("read: ");
	GPIO_ResetBits(GPIOB,HX711_SCK); //拉低SCK
	
	while(GPIO_ReadInputDataBit(GPIOB,HX711_DOUT));
	for(i=0;i<24;i++) 
	{ 
		delay_us(1);
		GPIO_SetBits(GPIOB,HX711_SCK);	   
		val=val<<1; 
		delay_us(1);
		GPIO_ResetBits(GPIOB,HX711_SCK);
		delay_us(1);
		a = GPIO_ReadInputDataBit(GPIOB,HX711_DOUT);
		//if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)){
		if(a){
		  val++;
		  //printf("read %x\r\n",val);
		}
		//printf(" %d",a);
	} 
  //printf("\r\n");
	GPIO_SetBits(GPIOB,HX711_SCK); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(GPIOB,HX711_SCK); 
	delay_us(1);
	
	return val-8388608;  //减去符号位导致的数据增大
}
