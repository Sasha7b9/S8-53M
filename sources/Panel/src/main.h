/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SL0_Pin GPIO_PIN_0
#define SL0_GPIO_Port GPIOA
#define SL1_Pin GPIO_PIN_1
#define SL1_GPIO_Port GPIOA
#define SL2_Pin GPIO_PIN_2
#define SL2_GPIO_Port GPIOA
#define SL3_Pin GPIO_PIN_3
#define SL3_GPIO_Port GPIOA
#define SL4_Pin GPIO_PIN_4
#define SL4_GPIO_Port GPIOA
#define SL5_Pin GPIO_PIN_5
#define SL5_GPIO_Port GPIOA
#define RL0_Pin GPIO_PIN_0
#define RL0_GPIO_Port GPIOB
#define RL1_Pin GPIO_PIN_1
#define RL1_GPIO_Port GPIOB
#define RL2_Pin GPIO_PIN_2
#define RL2_GPIO_Port GPIOB
#define ON_Pin GPIO_PIN_8
#define ON_GPIO_Port GPIOA
#define RC2_Pin GPIO_PIN_9
#define RC2_GPIO_Port GPIOA
#define RC1_Pin GPIO_PIN_10
#define RC1_GPIO_Port GPIOA
#define RC0_Pin GPIO_PIN_11
#define RC0_GPIO_Port GPIOA
#define RC3_Pin GPIO_PIN_12
#define RC3_GPIO_Port GPIOA
#define RL3_Pin GPIO_PIN_3
#define RL3_GPIO_Port GPIOB
#define RL4_Pin GPIO_PIN_4
#define RL4_GPIO_Port GPIOB
#define RL5_Pin GPIO_PIN_5
#define RL5_GPIO_Port GPIOB
#define RL6_Pin GPIO_PIN_6
#define RL6_GPIO_Port GPIOB
#define RL7_Pin GPIO_PIN_7
#define RL7_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
