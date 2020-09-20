#ifndef __HUMIDITY_H
#define __HUMIDITY_H	 
#include "sys.h"

/*================================================================
¡¾Function¡¿SHTC3µÄI2CµØÖ·
================================================================*/
#define SHTC3ADDR 0xE0

void humidity_init(void);
unsigned int ReadShtc3(void);
#endif
