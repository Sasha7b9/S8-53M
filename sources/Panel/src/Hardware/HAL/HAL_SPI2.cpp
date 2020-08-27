#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32l0xx_hal.h>


void HAL_SPI2::Init()
{
    SPI_HandleTypeDef handleSPI2 =
    {
        SPI2,
        {
            SPI_MODE_MASTER,
            SPI_DIRECTION_2LINES,
            SPI_DATASIZE_8BIT,
            SPI_POLARITY_LOW,
            SPI_PHASE_1EDGE,
            SPI_NSS_HARD_OUTPUT,
            SPI_BAUDRATEPRESCALER_2,
            SPI_FIRSTBIT_MSB,
            SPI_TIMODE_DISABLE,
            SPI_CRCCALCULATION_DISABLE,
            7
        }
    };

    HAL_SPI_Init(&handleSPI2);
}
