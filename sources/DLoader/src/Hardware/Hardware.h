#pragma once  
#include "defines.h"


#define PRIORITY_PANEL_EXTI9_5  3, 0
#define PRIORITY_PANEL_SPI1     4, 0


void Hardware_Init(void);

#define HARDWARE_ERROR HardwareErrorHandler(__FILE__, __FUNCTION__, __LINE__);
void HardwareErrorHandler(const char *file, const char *function, int line);
