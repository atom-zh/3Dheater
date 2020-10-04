#ifndef __BSP_CFG_H
#define __BSP_CFG_H	 
#include "sys.h"

typedef enum
{
    close,
    open
}Option;

typedef enum
{
    KEY_RELEASE,
    KEY_ENTER,
    KEY_LEFT,
    KEY_RIGHT
}KEY;

/*  status machine
 *  DisplayInfo: Just Display the Devices Info
 *  MenuEntry: Show the main Menu
 *  MenuCtrl: Star Menu contrl
 *  MenuExit: Return to DisplayInfo
 */
typedef enum 
{
    MENU_INFO,
    MENU_ENTRY,
    MENU_CTRL,
    MENU_EXIT
}DisplayStatus;

typedef struct
{
    int temp;
    int temp_th;
    int hum;
    int hum_th;
    
    char temp_mod;
    char hum_mod;

    int weight1_pi;
    int weight2_pi;
    float weight1;
    float weight2;
    
    KEY key_val;
    DisplayStatus status;
}Dev_Info;

//IO��������
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO��������	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 


void SPI2_Init(void);           //Ӳ��SPI2�ڳ�ʼ��
unsigned char SPI2_ReadWriteByte(unsigned char TxData);
#endif
