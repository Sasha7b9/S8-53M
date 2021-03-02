// 2021/03/02 14:09:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include <stm32f4xx_hal.h>


static SPI_HandleTypeDef handleSPI5 =       // Для связи с панелью
{
    SPI1, //-V2571
    {
        SPI_MODE_SLAVE,                 // Init.Mode
        SPI_DIRECTION_2LINES,           // Init.Direction
        SPI_DATASIZE_8BIT,              // Init.DataSize
        SPI_POLARITY_LOW,               // Init.CLKPolarity
        SPI_PHASE_2EDGE,                // Init.CLKPhase
        SPI_NSS_HARD_INPUT,             // Init.NSS
        SPI_BAUDRATEPRESCALER_32,       // Init.BaudRatePrescaler
        SPI_FIRSTBIT_MSB,               // Init.FirstBit
        SPI_TIMODE_DISABLED,            // Init.TIMode
        SPI_CRCCALCULATION_DISABLED,    // Init.CRCCalculation
        7                               // InitCRCPolynomial
    },
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};


void *HAL_SPI5::handle = &handleSPI5;


void HAL_SPI5::Init()
{
    HAL_PINS::InitSPI5();

    HAL_SPI_Init(&handleSPI5);
}


void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &handleSPI5)
    {

    }
}


void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &handleSPI5)
    {

    }
}
