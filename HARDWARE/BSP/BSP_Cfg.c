#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "BSP_Cfg.h"
#include "delay.h"

/***********    SPI2 interface ***********/
//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 输出高
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}


/***********    SPI2 interface ***********/
void SPI2_Init(void)           //硬件SPI2口初始化
{
  SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
   
  /* Enable SPI2 and GPIOB clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE) ;//使能SPI2的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能IO口时钟
  
  /* Configure SPI2 pins: SCK, MISO and MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure PB.12 as Output push-pull, used as Flash Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_ResetBits(GPIOB, GPIO_Pin_12);//片选信号为低
  
  /* SPI1 configuration */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 //设置为主SPI
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//SPI发送接收8位帧结构
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //时钟悬空高
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//数据捕获于第二个时钟沿
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	//内部NSS信号有SSI位控制
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//波特率预分频值为2
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输从MSB位开始
  SPI_InitStructure.SPI_CRCPolynomial = 7; //SPI_CRCPolynomial定义了用于CRC值计算的多项式
  SPI_Init(SPI2, &SPI_InitStructure);
  
  /* Enable SPI2  */
  SPI_Cmd(SPI2, ENABLE);
}

/*************************************************
函数功能：硬件SPI口发送或者接收一个字节数据

*************************************************/
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
unsigned char SPI2_ReadWriteByte(unsigned char TxData)
{		
	unsigned int retry=0;				 
	while((SPI2->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>2000)return 0;
	}			  
	SPI2->DR=TxData;	 	  //发送一个byte 
	retry=0;
	while((SPI2->SR&1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>2000)return 0;
	}	  						    
	return SPI2->DR;          //返回收到的数据				    
}

/*******************************************************/
/***************** 内部flash 操作接口 ******************/
/*******************************************************/
/* 参数存储 */
#define PARAM_SAVE_ADDR_BASE         0x0800F800
//#define PARAM_MAX_SIZE               (1*1024)//参数大小 1KB

#define PIECE_MAX_LEN  256

/* FLASH page size */
#define FLASH_PAGE_SIZE           0x400   //1KB
#define SAVE_DATA_MAGIC           0xCDAB3412

__IO FLASH_Status FLASHStatus = FLASH_COMPLETE;


void flash_erase_page(uint8_t flashPage, uint32_t addr_base)
{
	FLASHStatus = FLASH_COMPLETE;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(addr_base + flashPage*FLASH_PAGE_SIZE);
	FLASH_Lock();
}

void writeFlash(uint16_t * buf_to_save, uint16_t len, uint32_t wFlashAddr)
{
    uint16_t count = 0;
	FLASHStatus = FLASH_COMPLETE;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	while(count < len && FLASHStatus == FLASH_COMPLETE)
	{
		FLASH_ProgramHalfWord((wFlashAddr + count*2), buf_to_save[count]);
		count ++;
	}
	FLASH_Lock();
}

void flash_erase(uint32_t size , uint32_t addr_base)
{
    uint32_t flashPageSum;
	uint32_t i;
    if(size < FLASH_PAGE_SIZE)
    {
        size = FLASH_PAGE_SIZE;
    }

    if((size % FLASH_PAGE_SIZE) == 0)
    {
        flashPageSum = size/FLASH_PAGE_SIZE;
    }
    else
    {
        flashPageSum = (size/FLASH_PAGE_SIZE) + 1;
    }

    for(i = 0; i<flashPageSum; i++)
    {
        flash_erase_page(i, addr_base);
    }
}

void wFlashData(uint8_t * buf_to_save , uint16_t len , uint32_t wFlashAddr)
{
    uint8_t WriteFlashTempBuf[PIECE_MAX_LEN];
    uint16_t WriteFlashTempLen = 0;

    flash_erase(FLASH_PAGE_SIZE, PARAM_SAVE_ADDR_BASE);

    memset(WriteFlashTempBuf, 0xEE, sizeof(WriteFlashTempBuf));
    memcpy(WriteFlashTempBuf, buf_to_save, len);
    WriteFlashTempLen = len;
    if(len%2 != 0)
    {
        WriteFlashTempLen += 1;
    }
    writeFlash((uint16_t *)&WriteFlashTempBuf, WriteFlashTempLen/2, wFlashAddr);
}

void FLASH_ReadData(void *p_Readbuf)
{
	//p_Readbuf = (uint8_t *)(PARAM_SAVE_ADDR_BASE);
    printf("Read date from flash\n");
    memcpy(p_Readbuf, (uint8_t *)(PARAM_SAVE_ADDR_BASE), sizeof(struct DevParamSave));
}

void FLASH_SaveData(void *p_Writebuf)
{
    printf("Save date to flash \n");
	wFlashData(p_Writebuf, sizeof(struct DevParamSave), PARAM_SAVE_ADDR_BASE);
}

void Flash_Init(struct DevParamSave *p_Initbuf)
{
    if (p_Initbuf->magic == SAVE_DATA_MAGIC)
    {
        printf("Parameter already save\n");
    } else {
	    memset((uint8_t *)p_Initbuf, 0, sizeof(struct DevParamSave));
        p_Initbuf->magic = SAVE_DATA_MAGIC;
        p_Initbuf->temp_th = 30;
        p_Initbuf->hum_th = 50;
	    FLASH_SaveData((uint8_t*)p_Initbuf);
    }
}


/* Flash读写测试buf */
#define BufferSize 6
uint16_t usFlashWriteBuf[BufferSize] = {0x0101,0x0202,0x0303,0x0404,0x0505,0x0606};
uint16_t usFlashReadBuf[BufferSize] = {0};

/*******************************************************************************************************
** 函数: FlashReadWriteTest, 内部Flash读写测试函数
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: TEST_ERROR：错误（擦除、写入错误）  TEST_SUCCESS：成功   TEST_FAILED：失败
** 说明: 无
********************************************************************************************************/
int FlashReadWriteTest(void)
{
    int i;
    uint32_t ucStartAddr;

    /* 解锁 */
    FLASH_Unlock();

    /* 擦除操作 */
    ucStartAddr = PARAM_SAVE_ADDR_BASE;
    if (FLASH_COMPLETE != FLASH_ErasePage(ucStartAddr))
    {
        printf("Erase Error!\n");
        return -1;
    }
    else
    {
        ucStartAddr = PARAM_SAVE_ADDR_BASE;
        printf("擦除成功，此时FLASH中值为：\n");
        for (i = 0; i < BufferSize; i++)
        {
            printf("i = %d\n", i);
            usFlashReadBuf[i] = *(uint16_t*)ucStartAddr;
            printf("ucFlashReadBuf[%d] = 0x%.4x\n", i, usFlashReadBuf[i]);
            ucStartAddr += 2;
        }
    }

    /* 写入操作 */
    ucStartAddr = PARAM_SAVE_ADDR_BASE;
    printf("/n往FLASH中写入的数据为：\n");
    for (i = 0; i < BufferSize; i++)
    {
        if (FLASH_COMPLETE != FLASH_ProgramHalfWord(ucStartAddr, usFlashWriteBuf[i]))
        {
            printf("Write Error!\n");
            return -1;
        }
        printf("ucFlashWriteBuf[%d] = 0x%.4x\n", i, usFlashWriteBuf[i]);
        ucStartAddr += 2;
    }

    /* 上锁 */
    FLASH_Lock();

    /* 读取操作 */
    ucStartAddr = PARAM_SAVE_ADDR_BASE;
    printf("\n从FLASH中读出的数据为：\n");
    for (i = 0; i < BufferSize; i++)
    {
        usFlashReadBuf[i] = *(__IO uint16_t*)ucStartAddr;
        printf("ucFlashReadBuf[%d] = 0x%.4x\n", i, usFlashReadBuf[i]);
        ucStartAddr += 2;
    }

    /* 读出的数据与写入的数据做比较 */
    for (i = 0; i < BufferSize; i++)
    {
        if (usFlashReadBuf[i] != usFlashWriteBuf[i])
        {
            printf("\n对比失败！！！\n");
            return -1;
        }
    }

    return 0;
}