#ifndef __FAN_H
#define __FAN_H
#include "sys.h"
#include "BSP_Cfg.h"

#define GPIO_FAN (GPIO_Pin_13)

void Fan_Init(void);
void Fan_Ctrl(Option ctrl);
#endif
