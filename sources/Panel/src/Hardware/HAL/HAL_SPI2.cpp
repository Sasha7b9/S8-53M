#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32l0xx_hal.h>


static SPI_HandleTypeDef handleSPI2 =
{
    SPI2,
    {
        SPI_MODE_MASTER,
        SPI_DIRECTION_2LINES,
        SPI_DATASIZE_8BIT,
        SPI_POLARITY_HIGH,
        SPI_PHASE_2EDGE,
        SPI_NSS_HARD_OUTPUT,
        SPI_BAUDRATEPRESCALER_32,
        SPI_FIRSTBIT_MSB,
        SPI_TIMODE_DISABLE,
        SPI_CRCCALCULATION_DISABLED,
        7
    },
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};


void *HAL_SPI2::handle = &handleSPI2;


void HAL_SPI2::Init()
{
    HAL_PINS::InitSPI2();

    HAL_SPI_Init(&handleSPI2);
}


bool HAL_SPI2::Transmit(uint8 *buffer, int size)
{
    return HAL_SPI_Transmit(&handleSPI2, buffer, (uint16)size, 100) == HAL_OK;
}


void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &handleSPI2)
    {

    }
}


void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &handleSPI2)
    {

    }
}
