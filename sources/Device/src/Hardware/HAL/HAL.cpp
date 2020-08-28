#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <usbh_def.h>
#include <stm32f4xx_hal.h>


static void SystemClock_Config();


static HCD_HandleTypeDef handleHCD;
void *HAL_HCD::handle = &handleHCD;

static USBH_HandleTypeDef handleUSBH;
void *HAL_USBH::handle = &handleUSBH;

static PCD_HandleTypeDef handlePCD;
void *HAL_PCD::handle = &handlePCD;

static CRC_HandleTypeDef handleCRC = { CRC };


void HAL::Init()
{
    HAL_Init();

    SystemClock_Config();

    __SYSCFG_CLK_ENABLE();

    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();
    __GPIOH_CLK_ENABLE();
    __GPIOI_CLK_ENABLE();

    __ADC3_CLK_ENABLE();
    __CRC_CLK_ENABLE();
    __DAC_CLK_ENABLE();         // Для бикалки
    __DMA1_CLK_ENABLE();        // Для DAC1 (бикалка)
    __ETH_CLK_ENABLE();
    __PWR_CLK_ENABLE();
    __SPI1_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();
    __TIM2_CLK_ENABLE();        // Для тиков
    __TIM6_CLK_ENABLE();        // Для отсчёта миллисекунд
    __TIM7_CLK_ENABLE();        // Для DAC1 (бикалка)
    __USB_OTG_FS_CLK_ENABLE();
    __USB_OTG_HS_CLK_ENABLE();

    HAL_PINS::Init();

    HAL_ADC3::Init();

    HAL_DAC::Init();

    HAL_ETH::Init();

    HAL_HCD::Init();

    HAL_SPI1::Init();

    HAL_TIM6::Init();

    HAL_RTC::Init();

    HAL_SRAM::Init();

    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


static void SystemClock_Config()
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    // Enable HSE Oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 240;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 5;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        // Initialization Error
        HARDWARE_ERROR
    }
    // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        // Initialization Error
        HARDWARE_ERROR
    }
}


void HAL_HCD::Init()
{
    /* Set USBHS Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(OTG_HS_IRQn, 15, 0);

    /* Enable USBHS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}


void HAL_CRC::Init()
{
    HAL_CRC_Init(&handleCRC);
}


uint HAL_CRC::Calculate(uint address, uint numBytes)
{
    return HAL_CRC_Calculate(&handleCRC, (uint *)address, numBytes);
}

void HAL_ETH::Init()
{
    /* Output HSE clock (25MHz) on MCO pin (PA8) to clock the PHY */
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}
