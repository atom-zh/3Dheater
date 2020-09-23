//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途

//********************************************************************************
#ifndef __WEIGHT_H
#define __WEIGHT_H 

#include "stm32f10x.h"

void Init_Hx711(void);
unsigned long HX711_W1Read(void);
unsigned long HX711_W2Read(void);
#endif
