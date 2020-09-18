#ifndef __BSP_CFG_H
#define __BSP_CFG_H	 
#include "sys.h"



void SPI2_Init(void);           //硬件SPI2口初始化
unsigned char SPI2_ReadWriteByte(unsigned char TxData);

#endif
