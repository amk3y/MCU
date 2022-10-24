/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/

// Standard Library
#include <vector>
#include <deque>
#include <algorithm>

// User-Defined 
#include "main.h"



/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

int Number[10] = {
	0xFC00, 0x6000, 0xDA00,
	0xF200, 0x6600, 0xB600,
	0xBE00, 0xE000, 0xFE00, 
	0xE600
};

enum class KeyCode {
	
	KEY_0,
	KEY_1, KEY_2, KEY_3,
	KEY_4, KEY_5, KEY_6,
	KEY_7, KEY_8, KEY_9,
	KEY_A, KEY_B, KEY_C, KEY_D,
	KEY_ASTERISK, KEY_HASH,
	KEY_UNDEFINED
};

std::vector<KeyCode> scanKey(int row){
	std::vector<KeyCode> keys;
	switch (row){
		case 0: {
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == 0) keys.push_back(KeyCode::KEY_1);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) keys.push_back(KeyCode::KEY_2);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0) keys.push_back(KeyCode::KEY_3);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) keys.push_back(KeyCode::KEY_A);
			break;
		}
		case 1: {
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == 0) keys.push_back(KeyCode::KEY_4);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) keys.push_back(KeyCode::KEY_5);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0) keys.push_back(KeyCode::KEY_6);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) keys.push_back(KeyCode::KEY_B);
			break;
		}
		case 2: {
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == 0) keys.push_back(KeyCode::KEY_7);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) keys.push_back(KeyCode::KEY_8);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0) keys.push_back(KeyCode::KEY_9);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) keys.push_back(KeyCode::KEY_C);
			break;
		}
		case 3: {
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == 0) keys.push_back(KeyCode::KEY_ASTERISK);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) keys.push_back(KeyCode::KEY_0);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0) keys.push_back(KeyCode::KEY_HASH);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) keys.push_back(KeyCode::KEY_D);
			break;
		}
		default: {
			keys.push_back(KeyCode::KEY_UNDEFINED);
			break;
		}
	}
	return keys;
}

//
// | [0, 1, 2, 3] |
//

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
	
	
int numb[16]={0xfc00, 0x6000, 0xda00, 0xf200, 0x6600, 0xb600, 0xbe00, 0xe000, 0xfe00, 0xe600};
int button=1;

HAL_GPIO_WritePin(GPIOC,0xF00,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOB,0xFF00,GPIO_PIN_RESET);


while(1){
for(int i=0;i<2;i++){
	HAL_GPIO_WritePin(GPIOC,0x100<<i,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,numb[button],GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOB,0xFF00,GPIO_PIN_RESET);
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12)==0){
		switch(i){
			case 0:
				button=1;
				break;
			case 1:
				button=4;
				break;
		}
	}
	else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==0){
		switch(i){
			case 0:
				button=2;
				break;
			case 1:
				button=5;
				break;
		}
	}
HAL_GPIO_WritePin(GPIOC,0xF00,GPIO_PIN_SET);
}}

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 PB12 PB13 
                           PB14 PB15 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PC8 PC9 PC10 PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
