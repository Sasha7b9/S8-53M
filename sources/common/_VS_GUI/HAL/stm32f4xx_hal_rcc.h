// 2021/03/17 21:11:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "stm32f4xx_hal_def.h"


#define __HAL_RCC_PWR_CLK_ENABLE()
#define __HAL_RCC_RTC_ENABLE()
#define __DAC_CLK_ENABLE()
#define __DMA1_CLK_ENABLE()
#define __SYSCFG_CLK_ENABLE()
#define __GPIOA_CLK_ENABLE()
#define __GPIOB_CLK_ENABLE()
#define __GPIOC_CLK_ENABLE()
#define __GPIOD_CLK_ENABLE()
#define __GPIOE_CLK_ENABLE()
#define __GPIOF_CLK_ENABLE()
#define __GPIOG_CLK_ENABLE()
#define __GPIOH_CLK_ENABLE()
#define __GPIOI_CLK_ENABLE()
#define __ADC1_CLK_ENABLE()
#define __ADC3_CLK_ENABLE()
#define __CRC_CLK_ENABLE()
#define __DMA2D_CLK_ENABLE()
#define __ETH_CLK_ENABLE()
#define __FMC_CLK_ENABLE()
#define __LTDC_CLK_ENABLE()
#define __SPI1_CLK_ENABLE()
#define __SPI5_CLK_ENABLE()
#define __TIM2_CLK_ENABLE()
#define __TIM6_CLK_ENABLE()
#define __TIM7_CLK_ENABLE()
#define __USB_OTG_FS_CLK_ENABLE()
#define __USB_OTG_HS_CLK_ENABLE()


#define RCC_OSCILLATORTYPE_LSI 0
#define RCC_OSCILLATORTYPE_LSE 0
#define RCC_OSCILLATORTYPE_HSE 0
#define RCC_HSE_ON 0
#define RCC_PLL_ON 0
#define RCC_PLLSOURCE_HSE 0
#define RCC_PLLP_DIV2 0
#define RCC_PLL_NONE 0
#define RCC_LSE_ON 0
#define RCC_CLOCKTYPE_HCLK 0
#define RCC_CLOCKTYPE_SYSCLK 0
#define RCC_CLOCKTYPE_PCLK1 0
#define RCC_LSI_OFF 0
#define RCC_CLOCKTYPE_PCLK2 0
#define RCC_SYSCLKSOURCE_PLLCLK 0
#define RCC_SYSCLK_DIV1 0
#define RCC_HCLK_DIV4 0
#define RCC_HCLK_DIV2 0
#define RCC_PERIPHCLK_LTDC 0
#define RCC_PERIPHCLK_RTC 0
#define RCC_PLLSAIDIVR_2 0
#define RCC_RTCCLKSOURCE_LSE 0
#define RCC_MCO1 0
#define RCC_MCO1SOURCE_HSE 0
#define RCC_MCODIV_1 0


typedef struct
{
    uint32_t PLLState;   /*!< The new state of the PLL.
                              This parameter can be a value of @ref RCC_PLL_Config                      */

    uint32_t PLLSource;  /*!< RCC_PLLSource: PLL entry clock source.
                              This parameter must be a value of @ref RCC_PLL_Clock_Source               */

    uint32_t PLLM;       /*!< PLLM: Division factor for PLL VCO input clock.
                              This parameter must be a number between Min_Data = 0 and Max_Data = 63    */

    uint32_t PLLN;       /*!< PLLN: Multiplication factor for PLL VCO output clock.
                              This parameter must be a number between Min_Data = 50 and Max_Data = 432
                              except for STM32F411xE devices where the Min_Data = 192 */

    uint32_t PLLP;       /*!< PLLP: Division factor for main system clock (SYSCLK).
                              This parameter must be a value of @ref RCC_PLLP_Clock_Divider             */

    uint32_t PLLQ;       /*!< PLLQ: Division factor for OTG FS, SDIO and RNG clocks.
                              This parameter must be a number between Min_Data = 2 and Max_Data = 15    */
#if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || defined(STM32F446xx) || defined(STM32F469xx) ||\
    defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) ||\
    defined(STM32F413xx) || defined(STM32F423xx)
    uint32_t PLLR;       /*!< PLLR: PLL division factor for I2S, SAI, SYSTEM, SPDIFRX clocks.
                              This parameter is only available in STM32F410xx/STM32F446xx/STM32F469xx/STM32F479xx
                              and STM32F412Zx/STM32F412Vx/STM32F412Rx/STM32F412Cx/STM32F413xx/STM32F423xx devices.
                              This parameter must be a number between Min_Data = 2 and Max_Data = 7     */
#endif /* STM32F410xx || STM32F446xx || STM32F469xx || STM32F479xx || STM32F412Zx || STM32F412Vx || STM32F412Rx || STM32F412Cx || STM32F413xx || STM32F423xx */ 
}RCC_PLLInitTypeDef;


typedef struct
{
    uint32_t OscillatorType;       /*!< The oscillators to be configured.
                                        This parameter can be a value of @ref RCC_Oscillator_Type                   */

    uint32_t HSEState;             /*!< The new state of the HSE.
                                        This parameter can be a value of @ref RCC_HSE_Config                        */

    uint32_t LSEState;             /*!< The new state of the LSE.
                                        This parameter can be a value of @ref RCC_LSE_Config                        */

    uint32_t HSIState;             /*!< The new state of the HSI.
                                        This parameter can be a value of @ref RCC_HSI_Config                        */

    uint32_t HSICalibrationValue;  /*!< The HSI calibration trimming value (default is RCC_HSICALIBRATION_DEFAULT).
                                         This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F */

    uint32_t LSIState;             /*!< The new state of the LSI.
                                        This parameter can be a value of @ref RCC_LSI_Config                        */

    RCC_PLLInitTypeDef PLL;        /*!< PLL structure parameters                                                    */
}RCC_OscInitTypeDef;


typedef struct
{
    uint32_t PLLI2SM;    /*!< Specifies division factor for PLL VCO input clock.
                              This parameter must be a number between Min_Data = 2 and Max_Data = 63       */

    uint32_t PLLI2SN;    /*!< Specifies the multiplication factor for PLLI2S VCO output clock.
                              This parameter must be a number between Min_Data = 50 and Max_Data = 432    */

    uint32_t PLLI2SP;    /*!< Specifies division factor for SPDIFRX Clock.
                              This parameter must be a value of @ref RCCEx_PLLI2SP_Clock_Divider           */

    uint32_t PLLI2SQ;    /*!< Specifies the division factor for SAI clock.
                              This parameter must be a number between Min_Data = 2 and Max_Data = 15.
                              This parameter will be used only when PLLI2S is selected as Clock Source SAI */

    uint32_t PLLI2SR;    /*!< Specifies the division factor for I2S clock.
                              This parameter must be a number between Min_Data = 2 and Max_Data = 7.
                              This parameter will be used only when PLLI2S is selected as Clock Source I2S */
}RCC_PLLI2SInitTypeDef;


typedef struct
{
    uint32_t PLLSAIM;    /*!< Specifies division factor for PLL VCO input clock.
                              This parameter must be a number between Min_Data = 2 and Max_Data = 63       */

    uint32_t PLLSAIN;    /*!< Specifies the multiplication factor for PLLI2S VCO output clock.
                              This parameter must be a number between Min_Data = 50 and Max_Data = 432    */

    uint32_t PLLSAIP;    /*!< Specifies division factor for OTG FS, SDIO and RNG clocks.
                              This parameter must be a value of @ref RCCEx_PLLSAIP_Clock_Divider           */

    uint32_t PLLSAIQ;    /*!< Specifies the division factor for SAI clock.
                              This parameter must be a number between Min_Data = 2 and Max_Data = 15.
                              This parameter will be used only when PLLSAI is selected as Clock Source SAI */
    uint32_t PLLSAIR;
}RCC_PLLSAIInitTypeDef;


typedef struct
{
    uint32_t PeriphClockSelection; /*!< The Extended Clock to be configured.
                                        This parameter can be a value of @ref RCCEx_Periph_Clock_Selection */

    RCC_PLLI2SInitTypeDef PLLI2S;  /*!< PLL I2S structure parameters.
                                        This parameter will be used only when PLLI2S is selected as Clock Source I2S or SAI */

    RCC_PLLSAIInitTypeDef PLLSAI;  /*!< PLL SAI structure parameters.
                                        This parameter will be used only when PLLI2S is selected as Clock Source SAI or LTDC */

    uint32_t PLLI2SDivQ;           /*!< Specifies the PLLI2S division factor for SAI1 clock.
                                        This parameter must be a number between Min_Data = 1 and Max_Data = 32
                                        This parameter will be used only when PLLI2S is selected as Clock Source SAI */

    uint32_t PLLSAIDivQ;           /*!< Specifies the PLLI2S division factor for SAI1 clock.
                                        This parameter must be a number between Min_Data = 1 and Max_Data = 32
                                        This parameter will be used only when PLLSAI is selected as Clock Source SAI */

    uint32_t Sai1ClockSelection;    /*!< Specifies SAI1 Clock Source Selection.
                                        This parameter can be a value of @ref RCCEx_SAI1_Clock_Source */

    uint32_t Sai2ClockSelection;    /*!< Specifies SAI2 Clock Source Selection.
                                        This parameter can be a value of @ref RCCEx_SAI2_Clock_Source */

    uint32_t I2sApb1ClockSelection;    /*!< Specifies I2S APB1 Clock Source Selection.
                                        This parameter can be a value of @ref RCCEx_I2SAPB1_Clock_Source */

    uint32_t I2sApb2ClockSelection;    /*!< Specifies I2S APB2 Clock Source Selection.
                                        This parameter can be a value of @ref RCCEx_I2SAPB2_Clock_Source */

    uint32_t RTCClockSelection;      /*!< Specifies RTC Clock Source Selection.
                                        This parameter can be a value of @ref RCC_RTC_Clock_Source */

    uint32_t SdioClockSelection;    /*!< Specifies SDIO Clock Source Selection.
                                        This parameter can be a value of @ref RCCEx_SDIO_Clock_Source */

    uint32_t CecClockSelection;      /*!< Specifies CEC Clock Source Selection.
                                        This parameter can be a value of @ref RCCEx_CEC_Clock_Source */

    uint32_t Fmpi2c1ClockSelection;  /*!< Specifies FMPI2C1 Clock Source Selection.
                                        This parameter can be a value of @ref RCCEx_FMPI2C1_Clock_Source */

    uint32_t SpdifClockSelection;    /*!< Specifies SPDIFRX Clock Source Selection.
                                        This parameter can be a value of @ref RCCEx_SPDIFRX_Clock_Source */

    uint32_t Clk48ClockSelection;     /*!< Specifies CLK48 Clock Selection this clock used OTG FS, SDIO and RNG clocks.
                                        This parameter can be a value of @ref RCCEx_CLK48_Clock_Source */

    uint8_t TIMPresSelection;      /*!< Specifies TIM Clock Source Selection.
                                        This parameter can be a value of @ref RCCEx_TIM_PRescaler_Selection */
    uint32_t PLLSAIDivR;
}RCC_PeriphCLKInitTypeDef;


typedef struct
{
    uint32_t ClockType;             /*!< The clock to be configured.
                                         This parameter can be a value of @ref RCC_System_Clock_Type      */

    uint32_t SYSCLKSource;          /*!< The clock source (SYSCLKS) used as system clock.
                                         This parameter can be a value of @ref RCC_System_Clock_Source    */

    uint32_t AHBCLKDivider;         /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).
                                         This parameter can be a value of @ref RCC_AHB_Clock_Source       */

    uint32_t APB1CLKDivider;        /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK).
                                         This parameter can be a value of @ref RCC_APB1_APB2_Clock_Source */

    uint32_t APB2CLKDivider;        /*!< The APB2 clock (PCLK2) divider. This clock is derived from the AHB clock (HCLK).
                                         This parameter can be a value of @ref RCC_APB1_APB2_Clock_Source */

}RCC_ClkInitTypeDef;


__weak HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef *);
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *, uint32_t);
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef *);
void HAL_RCC_MCOConfig(uint32_t, uint32_t, uint32_t);
