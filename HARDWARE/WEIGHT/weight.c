#include "weight.h"
#include "delay.h"
#include "usart.h"

#define HX711_W1SCK		GPIO_Pin_0
#define HX711_W1DOUT    GPIO_Pin_1

#define HX711_W2SCK     GPIO_Pin_11
#define HX711_W2DOUT    GPIO_Pin_12

//****************************************************
// 初始化HX711
//****************************************************
void Init_Hx711(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    //weight1 SCK
    GPIO_InitStructure.GPIO_Pin = HX711_W1SCK ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //weight1 DOUT
    GPIO_InitStructure.GPIO_Pin = HX711_W1DOUT ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //weight2 SCK
    GPIO_InitStructure.GPIO_Pin = HX711_W2SCK ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //weight2 DOUT
    GPIO_InitStructure.GPIO_Pin = HX711_W2DOUT ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Init_Hx711_W2(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    //weight2 SCK
    GPIO_InitStructure.GPIO_Pin = HX711_W2SCK ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //weight2 DOUT
    GPIO_InitStructure.GPIO_Pin = HX711_W2DOUT ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//****************************************************
//读取 HX711
//****************************************************
unsigned long HX711_W1Read(void)
{
	unsigned long val = 0; 
	unsigned char i = 0; 
	unsigned char a = 0;

	//while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6));
	//delay_ms(10);
        //printf("read: ");
	GPIO_ResetBits(GPIOB,HX711_W1SCK); //拉低SCK
	
	while(GPIO_ReadInputDataBit(GPIOB,HX711_W1DOUT));
	for(i=0;i<24;i++) 
	{ 
		delay_us(1);
		GPIO_SetBits(GPIOB,HX711_W1SCK);	   
		val=val<<1; 
		delay_us(1);
		GPIO_ResetBits(GPIOB,HX711_W1SCK);
		delay_us(1);
		a = GPIO_ReadInputDataBit(GPIOB,HX711_W1DOUT);
		//if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)){
		if(a){
		  val++;
		  //printf("read %x\r\n",val);
		}
		//printf(" %d",a);
	} 
        //printf("\r\n");
	GPIO_SetBits(GPIOB,HX711_W1SCK); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(GPIOB,HX711_W1SCK); 
	delay_us(1);
	
	return val-8388608;  //减去符号位导致的数据增大
}

//****************************************************
//读取 HX711
//****************************************************
unsigned long HX711_W2Read(void)
{
	unsigned long val = 0; 
	unsigned char i = 0; 
	unsigned char a = 0;

	//while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6));
	//delay_ms(10);
        //printf("read: ");
	GPIO_ResetBits(GPIOA,HX711_W2SCK); //拉低SCK
	
	while(GPIO_ReadInputDataBit(GPIOA,HX711_W2DOUT));
	for(i=0;i<24;i++) 
	{ 
		delay_us(1);
		GPIO_SetBits(GPIOA,HX711_W2SCK);	   
		val=val<<1; 
		delay_us(1);
		GPIO_ResetBits(GPIOA,HX711_W2SCK);
		delay_us(1);
		a = GPIO_ReadInputDataBit(GPIOA,HX711_W2DOUT);
		//if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)){
		if(a){
		  val++;
		  //printf("read %x\r\n",val);
		}
		//printf(" %d",a);
	} 
        //printf("\r\n");
	GPIO_SetBits(GPIOA,HX711_W2SCK); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(GPIOA,HX711_W2SCK); 
	delay_us(1);
	
	return val-8388608;  //减去符号位导致的数据增大
}
