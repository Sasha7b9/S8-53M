#include "defines.h"
#include "common/Hardware/HAL/HAL_.h"
#include <usbh_def.h>
#include <stm32f4xx_hal.h>


static HCD_HandleTypeDef handleHCD;
void *HAL_HCD::handle = &handleHCD;

static USBH_HandleTypeDef handleUSBH;
void *HAL_USBH::handle = &handleUSBH;

static PCD_HandleTypeDef handlePCD;
void *HAL_PCD::handle = &handlePCD;


void HAL::Init()
{
    HAL_Init();

    ConfigSystemClock();

    EnablePeripherals();

    HAL_PINS::Init();

    HAL_SPI1::Init();

    HAL_SPI5::Init();

//    HAL_ADC3::Init();
//
    HAL_DAC2::Init();
//
//    HAL_RTC::Init();
//
//    HAL_TIM6::Init();

//    HAL_FMC::Init();


    HAL_TIM2::Init();

//    HAL_ETH::Init();

//    HAL_HCD::Init();

    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


void HAL_MspInit()
{
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    /* BusFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    /* UsageFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
    /* SVCall_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
    /* DebugMonitor_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


void HAL::EnablePeripherals()
{
#ifndef WIN32

    __SYSCFG_CLK_ENABLE();

    __PWR_CLK_ENABLE();

    __HAL_RCC_RTC_ENABLE();

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
    __DMA2D_CLK_ENABLE();

    __ETH_CLK_ENABLE();

    __FMC_CLK_ENABLE();

    __LTDC_CLK_ENABLE();

    __PWR_CLK_ENABLE();

    __SPI1_CLK_ENABLE();
    __SPI5_CLK_ENABLE();        // Свзяь с панелью
    __SYSCFG_CLK_ENABLE();

    __TIM2_CLK_ENABLE();        // Для тиков
    __TIM6_CLK_ENABLE();        // Для отсчёта миллисекунд
    __TIM7_CLK_ENABLE();        // Для DAC1 (бикалка)

    __USB_OTG_FS_CLK_ENABLE();
    __USB_OTG_HS_CLK_ENABLE();

#endif

}


void HAL::ConfigSystemClock()
{
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 15;
    RCC_OscInitStruct.PLL.PLLN = 144;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 5;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 60;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


void HAL_HCD::Init()
{
    /* Set USBHS Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(OTG_HS_IRQn, 15, 0);

    /* Enable USBHS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}


void HAL_ETH::Init()
{
    /* Output HSE clock (25MHz) on MCO pin (PA8) to clock the PHY */
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}


void HAL::SystemReset()
{
    HAL_NVIC_SystemReset();
}


void HAL::DeInit()
{
    HAL_DeInit();
}


void HAL::JumpToApplication()
{
#define MAIN_PROGRAM_START_ADDRESS  (uint)0x8020000

    typedef void(*pFunction)(void);

    pFunction JumpToApplication;

    __disable_irq();
    // Теперь переходим на основную программу
    JumpToApplication = (pFunction)(*(__IO uint *)(MAIN_PROGRAM_START_ADDRESS + 4)); //-V566 //-V2571
    __set_MSP(*(__IO uint *)MAIN_PROGRAM_START_ADDRESS); //-V566 //-V2571
    __enable_irq();
    JumpToApplication();
}


void HAL::ErrorHandler(const char *file, int line)
{
    UNUSED(file);
    UNUSED(line);

    while (true)
    {
    }
}
