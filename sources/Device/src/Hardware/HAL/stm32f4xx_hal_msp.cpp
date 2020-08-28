#include "Hardware/Hardware.h"
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_sram.h>



void HAL_SPI_MspInit(SPI_HandleTypeDef *)
{

}


void HAL_SPI_MspDeInit(SPI_HandleTypeDef *)
{

}


void HAL_SRAM_MspInit(SRAM_HandleTypeDef *)
{
//    __FSMC_CLK_ENABLE();
//    __GPIOB_CLK_ENABLE();
//    __GPIOD_CLK_ENABLE();
//    __GPIOE_CLK_ENABLE();
//
//    GPIO_InitTypeDef isGPIOD = {
//        GPIO_PIN_0 |        // DA2
//        GPIO_PIN_1 |        // DA3
//        GPIO_PIN_4 |        // RD
//        GPIO_PIN_5 |        // WR
//        GPIO_PIN_7 |        // NE1
//        GPIO_PIN_11 |       // A16
//        GPIO_PIN_12 |       // A17
//        GPIO_PIN_13 |       // A18
//        GPIO_PIN_14 |       // DA0
//        GPIO_PIN_15,        // DA1
//        GPIO_MODE_AF_PP,
//        GPIO_NOPULL,
//        GPIO_SPEED_FAST,
//        GPIO_AF12_FSMC
//    };
//    HAL_GPIO_Init(GPIOD, &isGPIOD);
//
//    GPIO_InitTypeDef isGPIOE_B = {
//        GPIO_PIN_2 |        // A23
//        GPIO_PIN_3 |        // A19
//        GPIO_PIN_4 |        // A20
//        GPIO_PIN_5 |        // A21
//        GPIO_PIN_6 |        // A22
//        GPIO_PIN_7 |        // DA4
//        GPIO_PIN_8 |        // DA5
//        GPIO_PIN_9 |        // DA6
//        GPIO_PIN_10,        // DA7
//        GPIO_MODE_AF_PP,
//        GPIO_NOPULL,
//        GPIO_SPEED_FAST,
//        GPIO_AF12_FSMC
//    };
//    HAL_GPIO_Init(GPIOE, &isGPIOE_B);
//
//    isGPIOE_B.Pin = GPIO_PIN_7;     // NL1
//    HAL_GPIO_Init(GPIOB, &isGPIOE_B);
}


void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef *)
{
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_11 ||
                    GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
}


void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
    static DMA_HandleTypeDef hdmaDAC1 =
    {
        DMA1_Stream5,
        {
            DMA_CHANNEL_7,
            DMA_MEMORY_TO_PERIPH,
            DMA_PINC_DISABLE,
            DMA_MINC_ENABLE,
            DMA_PDATAALIGN_BYTE,
            DMA_MDATAALIGN_BYTE,
            DMA_CIRCULAR,
            DMA_PRIORITY_HIGH,
            DMA_FIFOMODE_DISABLE,
            DMA_FIFO_THRESHOLD_HALFFULL,
            DMA_MBURST_SINGLE,
            DMA_PBURST_SINGLE
        }
    };

    HAL_DMA_Init(&hdmaDAC1);

    __HAL_LINKDMA(hdac, DMA_Handle1, hdmaDAC1);

    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}


void HAL_DAC_MspDeInit(DAC_HandleTypeDef *)
{

}


void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    if (heth->Init.MediaInterface == ETH_MEDIA_INTERFACE_MII)
    {
        /* Output HSE clock (25MHz) on MCO pin (PA8) to clock the PHY */
        HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
    }
}


void HAL_CRC_MspInit(CRC_HandleTypeDef *)
{
    __HAL_RCC_CRC_CLK_ENABLE();
}


void HAL_CRC_MspDeInit(CRC_HandleTypeDef *)
{
    __HAL_RCC_CRC_FORCE_RESET();
    __HAL_RCC_CRC_RELEASE_RESET();
}
