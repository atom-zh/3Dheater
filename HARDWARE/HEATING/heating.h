#ifndef __HEATING_H
#define __HEATING_H
#include "sys.h"
#include "BSP_Cfg.h"

#define GPIO_HEATING (GPIO_Pin_15)

void Heating_Init(void);
void Heating_Ctrl(Option ctrl);
#endif
