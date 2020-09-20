#ifndef __TEMP_H
#define __TEMP_H	
#include "sys.h"

#define	Success			0x01
#define	Fail				0x00
#define NTC_10K			10    //NTC_10K传感器
#define NTC_50K			50    //NTC_50K传感器

#define Length_10K				140   	 //240是Temp_Tab_10K数组的长度
#define Length_50K	      179    //179是Temp_Tab_50K数组的长度
#define Power_Vol					3.3			//3.3V是串联电路中的电源电压，可根据实际电路修改
#define RES_10K						10			//单位kΩ；温度传感器串联分压电阻，可根据实际电路修改
#define RES_50K						51			//单位kΩ；温度传感器串联分压电阻，可根据实际电路修改

void Temp_Init(void);
float Get_Temp(void); 
 
#endif 
