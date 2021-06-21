#include "stm32f4xx_hal.h"

DAC_HandleTypeDef    DacHandle;
static DAC_ChannelConfTypeDef sConfig;
const uint8_t aEscalator8bit[6] = {0x0, 4, 4, 4, 4, 0};
__IO uint8_t ubSelectedWavesForm = 1;
__IO uint8_t keyPressed = 1; 

static void DAC_Ch1_TriangleConfig(void);
static void DAC_Ch1_EscalatorConfig(void);
static void TIM6_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);

int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  /*##-1- Configure the DAC peripheral #######################################*/
  DacHandle.Instance = DAC;
  
  /*##-2- Configure the TIM peripheral #######################################*/
  TIM6_Config();
  
  /* Infinite loop */
  while (1)
  {
    /* If USER Button is pressed */
    if (keyPressed == 1)
    {
      HAL_DAC_DeInit(&DacHandle);
      
//      DAC_Ch1_TriangleConfig();

      DAC_Ch1_EscalatorConfig();
      
      keyPressed = 0; 
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
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
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 60;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
  }
}

static void DAC_Ch1_EscalatorConfig(void)
{
  /*##-1- Initialize the DAC peripheral ######################################*/
  if(HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-1- DAC channel1 Configuration #########################################*/
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;  

  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }
  
  /*##-2- Enable DAC Channel1 and associated DMA #############################*/
  if(HAL_DAC_Start_DMA(&DacHandle, DAC_CHANNEL_2, (uint32_t*)aEscalator8bit, 6, DAC_ALIGN_8B_R) != HAL_OK)
  {
    /* Start DMA Error */
    Error_Handler();
  }
}

/**
  * @brief  DAC Channel1 Triangle Configuration
  * @param  None
  * @retval None
  */
static void DAC_Ch1_TriangleConfig(void)
{
  /*##-1- Initialize the DAC peripheral ######################################*/
  if(HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-2- DAC channel2 Configuration #########################################*/
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;  

  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }
  
  /*##-3- DAC channel2 Triangle Wave generation configuration ################*/
  if(HAL_DACEx_TriangleWaveGenerate(&DacHandle, DAC_CHANNEL_2, DAC_TRIANGLEAMPLITUDE_1023) != HAL_OK)
  {
    /* Triangle wave generation Error */
    Error_Handler();
  }
  
  /*##-4- Enable DAC Channel1 ################################################*/
  if(HAL_DAC_Start(&DacHandle, DAC_CHANNEL_2) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /*##-5- Set DAC channel1 DHR12RD register ##################################*/
  if(HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x100) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Change the wave */
    keyPressed = 1;

  /* Change the selected waves forms */
  ubSelectedWavesForm = !ubSelectedWavesForm;
}

/**             
  * @brief  TIM6 Configuration
  * @note   TIM6 configuration is based on APB1 frequency
  * @note   TIM6 Update event occurs each TIM6CLK/256   
  * @param  None
  * @retval None
  */
void TIM6_Config(void)
{
  static TIM_HandleTypeDef htim;
  TIM_MasterConfigTypeDef  sMasterConfig;
  
  /*##-1- Configure the TIM peripheral #######################################*/
  /* Time base configuration */
  htim.Instance = TIM6;
  
  htim.Init.Period = 0x7FF;          
  htim.Init.Prescaler = 0;       
  htim.Init.ClockDivision = 0;    
  htim.Init.CounterMode = TIM_COUNTERMODE_UP; 
  HAL_TIM_Base_Init(&htim);

  /* TIM6 TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  
  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);
  
  /*##-2- Enable TIM peripheral counter ######################################*/
  HAL_TIM_Base_Start(&htim);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
