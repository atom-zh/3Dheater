#ifndef __HUMIDITY_H
#define __HUMIDITY_H	 
#include "sys.h"

/*================================================================
【Function】SHTC3的I2C地址
================================================================*/
#define SHTC3ADDR 0xE0

void humidity_init(void);
unsigned int ReadShtc3(void);
#endif
