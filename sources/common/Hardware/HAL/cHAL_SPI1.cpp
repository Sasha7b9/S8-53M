#include "defines.h"
#include "common/Hardware/HAL/HAL.h"
#include "Panel/Panel.h"
#include <stm32f4xx_hal.h>


static SPI_HandleTypeDef handleSPI =
{
    SPI1,
    {
        SPI_MODE_SLAVE,                 // Init.Mode
        SPI_DIRECTION_2LINES,           // Init.Direction
        SPI_DATASIZE_8BIT,              // Init.DataSize
        SPI_POLARITY_HIGH,              // Init.CLKPolarity
        SPI_PHASE_1EDGE,                // Init.CLKPhase
        SPI_NSS_SOFT,                   // Init.NSS
        SPI_BAUDRATEPRESCALER_2,        // Init.BaudRatePrescaler
        SPI_FIRSTBIT_MSB,               // Init.FirstBit
        SPI_TIMODE_DISABLED,            // Init.TIMode
        SPI_CRCCALCULATION_DISABLED,    // Init.CRCCalculation
        7                               // InitCRCPolynomial
    }
};

void *HAL_SPI1::handle = &handleSPI;


void HAL_SPI1::Init()
{
    HAL_SPI_Init(&handleSPI);

    HAL_NVIC_SetPriority(SPI1_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    // Теперь настроим программный NSS (PG0). pinSPI1_NSS

    HAL_NVIC_SetPriority(EXTI0_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void HAL_SPI1::DeInit()
{
    HAL_NVIC_DisableIRQ(SPI1_IRQn);
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
}


static uint8 dataSPIfromPanel;

void HAL_GPIO_EXTI_Callback(uint16 pin)
{
    if (pin == GPIO_PIN_0)
    {
        HAL_SPI_Receive_IT(&handleSPI, &dataSPIfromPanel, 1);
    }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *handle)
{
    if (!Panel::ProcessingCommandFromPIC(dataSPIfromPanel))
    {
        HAL_SPI_DeInit(handle);
        HAL_SPI_Init(handle);
    }
    uint16 data = Panel::NextData();
    SPI1->DR = data;
}
