#ifndef __BSP_CFG_H
#define __BSP_CFG_H	 
#include "sys.h"
#include "usart.h"

#define HUM_MIN         10
#define TEMP_MAX        100

typedef enum
{
    close,
    open
}Option;

enum KEY
{
    KEY_RELEASE,
    KEY_ENTER,
    KEY_LEFT,
    KEY_RIGHT
}__attribute__((__packed__));

/*  status machine
 *  DisplayInfo: Just Display the Devices Info
 *  MenuEntry: Show the main Menu
 *  MenuCtrl: Star Menu contrl
 *  MenuExit: Return to DisplayInfo
 */
enum DisplayStatus
{
    MENU_INFO,
    MENU_ENTRY,
    MENU_CTRL,
    MENU_EXIT
}__attribute__((__packed__));

struct DevParamSave
{
    vu32 magic;
    char temp_mod;
    char hum_mod;

    int temp_th;
    int hum_th;

    int w1_offset;
    int w2_offset;
}__attribute__((__packed__));

struct Dev_Info
{
    char temp_mod;
    char hum_mod;

    int temp;
    int hum;

    float weight1;
    float weight2;

    struct DevParamSave set;
    enum KEY key_val;
    enum DisplayStatus status;
}__attribute__((__packed__));

//IO方向设置
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 


void SPI2_Init(void);           //硬件SPI2口初始化
unsigned char SPI2_ReadWriteByte(unsigned char TxData);

int FlashReadWriteTest(void);
void Flash_Init(struct DevParamSave *p_Initbuf);
void FLASH_ReadData(void *p_Readbuf);
void FLASH_SaveData(void *p_Writebuf);

#endif
