#include "stm32f10x.h"
#include "BSP_Cfg.h"
#include "humidity.h"
#include "delay.h"
#include "usart.h"


#define ACK         1             //应答信号
#define NACK        0             //非应答信号

void humidity_init(void)
{
    IIC_Init();
}



/*================================================================
【  Name  】float ReadShtc3(char whatdo)
【Function】非主机模式，读取函数函数
【  Notes 】
================================================================*/
unsigned int ReadShtc3(void)
{
    //float Humidity,Temperature;
    unsigned int HumData,TempData;

    IIC_Start();        
    IIC_Send_Byte(SHTC3ADDR&0xfe);           //I2C address + write + ACK  
    IIC_Wait_Ack();
    IIC_Send_Byte(0X35);                     //Wakeup MSB  唤醒
    IIC_Wait_Ack();
    IIC_Send_Byte(0X17);                     //Wakeup LSB
    IIC_Wait_Ack();
    IIC_Stop();  
    delay_ms(10);      
    
    IIC_Start();
    IIC_Send_Byte(SHTC3ADDR&0xfe);           //I2C address + write + ACK   	
    IIC_Wait_Ack();
    IIC_Send_Byte(0X58);                     //Measurement command MSB 湿度值优先 不锁存SCL时钟线
    IIC_Wait_Ack();
    IIC_Send_Byte(0XE0);                     //Measurement command LSB
    IIC_Wait_Ack();
    IIC_Stop(); 
    delay_ms(50); 														//等待测量完成
    
    IIC_Start();
    IIC_Send_Byte(SHTC3ADDR|0x01);     			//I2C address + read  可通过ACK来判断是否测量完成。完成则有响应
    IIC_Wait_Ack();
    delay_ms(1); 
    HumData = IIC_Read_Byte(ACK);             //Data(MSB) 先读湿度
    HumData=HumData<<8;
    HumData |= IIC_Read_Byte(ACK);            //Data(LSB)
    IIC_Read_Byte(ACK);  

    TempData = IIC_Read_Byte(ACK);            //Data(MSB) 后读温度
    TempData=TempData<<8;
    TempData |= IIC_Read_Byte(ACK);           //Data(LSB)
    IIC_Read_Byte(NACK); 
    IIC_Stop();                                //Stop I2C
		
    IIC_Start();        
    IIC_Send_Byte(SHTC3ADDR&0xfe);           //I2C address + write + ACK  
    IIC_Wait_Ack();        
    IIC_Send_Byte(0XB0);                     //Sleep MSB 进入休眠
    IIC_Wait_Ack();
    IIC_Send_Byte(0X98);                     //Sleep LSB
    IIC_Wait_Ack();
    IIC_Stop();  

    /*-- calculate relative humidity [%RH] --*/ 
    HumData =(HumData*100.0)/65536;                 //公式: RH%=100 * SRH/2^16                  
    DEBUG("Hum:%f \r\n", Humidity);
    
    /*-- calculate temperature [°C] --*/
    TempData = (TempData*175.0)/65536-45;      //公式:T= -45 + 175 * ST/2^16       
    DEBUG("Temp:%f \r\n", Temperature);
    return HumData;
}