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
#include "main.h"
#include <math.h>
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
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */



int Number[10] = {
	0xFC00, 0x6000, 0xDA00,
	0xF200, 0x6600, 0xB600,
	0xBE00, 0xE000, 0xFE00, 
	0xE600
};

enum KeyCode {
	
	KEY_0,
	KEY_1, KEY_2, KEY_3,
	KEY_4, KEY_5, KEY_6,
	KEY_7, KEY_8, KEY_9,
	KEY_A, KEY_B, KEY_C, KEY_D,
	KEY_ASTERISK, KEY_HASH,
	KEY_UNDEFINED
};

int translateKeyCodeToLED(enum KeyCode code){
	if((int) code < 10)
		return Number[(int) code];
	else if(code == KEY_A)
		return 0xEE00;
	else if(code == KEY_B)
		return 0x3E00;
	else if(code == KEY_C)
        //0001 1010
		return 0x1A00;
	else if(code == KEY_D)
			// 0111 1010
		return 0x7A00;
	else if(code == KEY_ASTERISK)
		//1001 1110
		return 0x9E00;
	else if(code == KEY_HASH)
		//1000 1110
		return 0x8E00;
	else 
		return 0x9000;
}

enum KeyCode scanKey(int row){
	switch (row){
		case 0: {
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == 0) return KEY_1;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) return KEY_2;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0) return KEY_3;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) return KEY_A;
			else return KEY_UNDEFINED;
		}
		case 1: {
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == 0) return KEY_4;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) return KEY_5;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0) return KEY_6;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) return KEY_B;
			else return KEY_UNDEFINED;
		}
		case 2: {
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == 0) return KEY_7;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) return KEY_8;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0) return KEY_9;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) return KEY_C;
			else return KEY_UNDEFINED;
		}
		case 3: {
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == 0) return KEY_ASTERISK;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) return KEY_0;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0) return KEY_HASH;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) return KEY_D;
			else return KEY_UNDEFINED;
		}
		default: {
			return KEY_UNDEFINED;
		}
	}
}



volatile int value = 0;
enum KeyCode lastKeyOfRow[4] = {KEY_UNDEFINED};
enum KeyCode currentKeyOfRow[4] = {KEY_UNDEFINED};
int64_t keyLastUpdate = 0L; 

volatile int selectedIndex = 3;
volatile int scanningIndex = 0;

int timerEnabled = 0;
int timerEnded = 0;


void tick_timer_2(){

}

void tick_timer_3(){
	// Reset Scan Lines -> GPIOC 8 ~ 11  
	HAL_GPIO_WritePin(GPIOC, 0x0F00, GPIO_PIN_SET);
	// Reset LED Lines 
	HAL_GPIO_WritePin(GPIOB, 0xFF00, GPIO_PIN_RESET);
	
	// Set Current Scan Line (From PC8 to PC11)
	// 0000 1111 0000 0000
	HAL_GPIO_WritePin(GPIOC, 0x0100 << scanningIndex, GPIO_PIN_RESET);
	enum KeyCode keycode = scanKey(scanningIndex);
	
	//Debounce 
	if(keycode != lastKeyOfRow[scanningIndex]){
			keyLastUpdate = HAL_GetTick();
	}
	
	if((HAL_GetTick() - keyLastUpdate) > 70){
				if(keycode != currentKeyOfRow[scanningIndex]){
					currentKeyOfRow[scanningIndex] = keycode;
					if(keycode != KEY_UNDEFINED){
						// Button Action
						if(keycode == KEY_HASH){
							selectedIndex--;
							if(selectedIndex < 0) selectedIndex = 3;
						}else if(keycode == KEY_ASTERISK){
						  // TODO: Start Timer;
						}else if(((int) keycode) < 10){
							// 1234 -> 1334
							// 1234 - 200 = 1034
							// 1034 + 300 = 1334
							value = value - ( (int) (value * pow(10, -selectedIndex)) % 10 *  pow(10, selectedIndex))
							+ (keycode * pow(10, selectedIndex));
						}
						// Button Action End
					}
				}
			}
	
			lastKeyOfRow[scanningIndex] = keycode;
			//Debounce End
	

			
		int displayArray[4] = {0, 0, 0, 0};
		displayArray[3] =  value  / 1000 % 10;
		displayArray[2] = (value / 100) % 10;
		displayArray[1] = (value / 10) % 10;
		displayArray[0] = value % 10;
			//
			// Do Display
			//
		HAL_GPIO_WritePin(GPIOB, 0xFF00, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Number[displayArray[scanningIndex]], GPIO_PIN_SET);
		
	
		scanningIndex++;
		if(scanningIndex >= 4) scanningIndex = 0;

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM3){
		tick_timer_3();
	}else if(htim->Instance == TIM2){
		tick_timer_2();
	}
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM3_Init();
	HAL_TIM_Base_Start_IT(&htim3);

	
  while (1)
  {

  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 47999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12 
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8 
                          |GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 PB10 PB11 PB12 
                           PB13 PB14 PB15 PB8 
                           PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12 
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8 
                          |GPIO_PIN_9;
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
