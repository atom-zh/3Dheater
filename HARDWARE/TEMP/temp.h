#ifndef __TEMP_H
#define __TEMP_H	
#include "sys.h"

void Temp_Init(void);
u16  Get_Temp(u8 ch); 
u16 Get_Temp_Average(u8 ch,u8 times); 
 
#endif 
