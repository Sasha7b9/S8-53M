#include "Hardware/HAL/HAL.h"
#include <stm32l0xx_hal.h>


SPI_HandleTypeDef hspi2;


int main(void)
{
    HAL_Init();

    HAL::Init();

    while (1)
    {
    }
}


#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
