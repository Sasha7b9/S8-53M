#include "defines.h"
#include "common/Hardware/HAL/HAL_c.h"
#include <usbh_def.h>
#include <stm32f4xx_hal.h>


static void SystemClock_Config();


static HCD_HandleTypeDef handleHCD;
void *HAL_HCD::handle = &handleHCD;

static USBH_HandleTypeDef handleUSBH;
void *HAL_USBH::handle = &handleUSBH;

static PCD_HandleTypeDef handlePCD;
void *HAL_PCD::handle = &handlePCD;


void HAL::Init()
{
    HAL_Init();

    SystemClock_Config();

    __SYSCFG_CLK_ENABLE();      //-V2571

    __GPIOA_CLK_ENABLE();       //-V2571
    __GPIOB_CLK_ENABLE();       //-V2571
    __GPIOC_CLK_ENABLE();       //-V2571
    __GPIOD_CLK_ENABLE();       //-V2571
    __GPIOE_CLK_ENABLE();       //-V2571
    __GPIOF_CLK_ENABLE();       //-V2571
    __GPIOG_CLK_ENABLE();       //-V2571
    __GPIOH_CLK_ENABLE();       //-V2571
    __GPIOI_CLK_ENABLE();       //-V2571

    __ADC3_CLK_ENABLE();        //-V2571
    __CRC_CLK_ENABLE();         //-V2571
    __DAC_CLK_ENABLE();         //-V2571 ��� ������� 
    __DMA1_CLK_ENABLE();        //-V2571 ��� DAC1 (�������)
    __ETH_CLK_ENABLE();         //-V2571
    __FMC_CLK_ENABLE();         //-V2571
    __PWR_CLK_ENABLE();         //-V2571
    __SPI1_CLK_ENABLE();        //-V2571
    __SYSCFG_CLK_ENABLE();      //-V2571
    __TIM2_CLK_ENABLE();        //-V2571 ��� �����
    __TIM6_CLK_ENABLE();        //-V2571 ��� ������� �����������
    __TIM7_CLK_ENABLE();        //-V2571 ��� DAC1 (�������)
    __USB_OTG_FS_CLK_ENABLE();  //-V2571
    __USB_OTG_HS_CLK_ENABLE();  //-V2571

    HAL_PINS::Init();


#ifdef DEVICE

    HAL_ADC3::Init();

    HAL_DAC::Init();

    HAL_RTC::Init();

    HAL_TIM6::Init();

#endif

    HAL_FMC::Init();

    HAL_TIM2::Init();

    HAL_ETH::Init();

    HAL_HCD::Init();

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
        ERROR_HANDLER();
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
    // ������ ��������� �� �������� ���������
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
