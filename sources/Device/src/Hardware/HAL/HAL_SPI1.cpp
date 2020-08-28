#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Panel/Panel.h"
#include <stm32f4xx_hal.h>


void HAL_SPI1::Init()
{
    HAL_SPI_Init(reinterpret_cast<SPI_HandleTypeDef *>(handleSPI));

    HAL_NVIC_SetPriority(SPI1_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    // Теперь настроим программный NSS (PG0). pinSPI1_NSS

    HAL_NVIC_SetPriority(EXTI0_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

static uint8 dataSPIfromPanel;

void HAL_GPIO_EXTI_Callback(uint16 pin)
{
    if (pin == GPIO_PIN_0)
    {
        HAL_SPI_Receive_IT(reinterpret_cast<SPI_HandleTypeDef *>(handleSPI), &dataSPIfromPanel, 1);
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
