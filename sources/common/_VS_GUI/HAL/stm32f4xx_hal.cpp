#include "defines.h"
#include <stm32f4xx_hal.h>


__weak void HAL_Delay(uint32_t)
{

}

HAL_StatusTypeDef HAL_Init()
{
    return HAL_ERROR;
}


HAL_StatusTypeDef HAL_DeInit()
{
    return HAL_ERROR;
}


__weak uint32_t HAL_GetTick()
{
    return 0;
}