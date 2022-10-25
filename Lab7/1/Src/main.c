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

int translateKeyCodeToLED(KeyCode code){
	if((int) code < 10)
		return Number[(int) code];
	else if(code == KeyCode::KEY_A)
		return 0xEE00;
	else if(code == KeyCode::KEY_B)
		return 0x3E00;
	else if(code == KeyCode::KEY_C)
			//1001 1100
		return 0x9C00;
	else if(code == KeyCode::KEY_D)
			// 0111 1010
		return 0x7A00;
	else if(code == KeyCode::KEY_ASTERISK)
		//1001 1110
		return 0x9E00;
	else if(code == KeyCode::KEY_HASH)
		//1000 1110
		return 0x8E00;
	else 
		return 0x9000;
}

KeyCode scanKey(int row){
	switch (row) {
		case 0: {
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == 0) return KeyCode::KEY_1;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) return KeyCode::KEY_2;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0) return KeyCode::KEY_3;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) return KeyCode::KEY_A;
			else return KeyCode::KEY_UNDEFINED;
		}
		case 1: {
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == 0) return KeyCode::KEY_4;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) return KeyCode::KEY_5;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0) return KeyCode::KEY_6;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) return KeyCode::KEY_B;
			else return KeyCode::KEY_UNDEFINED;
		}
		case 2: {
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == 0) return KeyCode::KEY_7;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) return KeyCode::KEY_8;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0) return KeyCode::KEY_9;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) return KeyCode::KEY_C;
			else return KeyCode::KEY_UNDEFINED;
		}
		case 3: {
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == 0) return KeyCode::KEY_ASTERISK;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0) return KeyCode::KEY_0;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_14) == 0) return KeyCode::KEY_HASH;
			else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0) return KeyCode::KEY_D;
			else return KeyCode::KEY_UNDEFINED;
		}
		default: {
			return KeyCode::KEY_UNDEFINED;
		}
	}
}

/**
*
* No STL ?
* Make one yourself :)
*
*/
template<typename T>
class FixedQueue {

public:
    FixedQueue(int capacity)
    : _maxCapacity(capacity)
    , _data(new T[capacity])
    , _size(0) {

    };

    ~FixedQueue(){
        free(_data);
    };

    T *poll() {
        if (_size == 0) return nullptr;
        return &_data[--_size];

    }

    void offer(T data) {
        _size = _size < _maxCapacity ? _size + 1 : _maxCapacity;
       // printf("%d %d %d %d %d ", _data[0], _data[1], _data[2], _data[3], _data[4]);
        for (int i = _size - 1; i > 0; i--) {
            _data[i] = _data[i - 1];
        }
        _data[0] = data;
    }

    T operator[](int index) {
        return _data[index];
    }

    int size(){
        return _size;
    }

    int capacity(){
        return _maxCapacity;
    }
private:
    T* _data;
    int _size;
    const int _maxCapacity;
};

//
// | [0, 1, 2, 3] |
//


int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

	/**
	* Pin Definition
	* 
	* GPIOB -> Seven Segement Display LED
	*
	*
	*
	*/
	
	//Debounce Variables
	KeyCode lastKeyOfRow[4] = {KeyCode::KEY_UNDEFINED};
	KeyCode currentKeyOfRow[4] = {KeyCode::KEY_UNDEFINED};
	long long keyLastUpdate = HAL_GetTick(); 
	//
	KeyCode kk = KeyCode::KEY_0;
	
	FixedQueue<int> displayQueue(4);
	for(int i = 0; i < 4; i++){
		displayQueue.offer(0);
	}
	
  while (true)
  {
		// Reset All State
		HAL_GPIO_WritePin(GPIOC, 0xFF00, GPIO_PIN_RESET);
		HAL_Delay(1);
		
		
		//
		//  Every scanning overrides last result!
		//  You must take this into account.
		//
		for(int i = 0; i < 4; i++){
			// Reset Scan Lines -> GPIOC 8 ~ 11  
			HAL_GPIO_WritePin(GPIOC, 0x0F00, GPIO_PIN_SET);
			// Set Current Scan Line (From PC8 to PC11)
			// 0000 1111 0000 0000
			HAL_GPIO_WritePin(GPIOC, 0x0100 << i, GPIO_PIN_RESET);
			HAL_Delay(1);
			auto keycode = scanKey(i);
		
			//Debounce 
			if(keycode != lastKeyOfRow[i]){
				keyLastUpdate = HAL_GetTick();
			}
			//40 = debounce delay
			if((HAL_GetTick() - keyLastUpdate) > 40){
				if(keycode != currentKeyOfRow[i]){
					currentKeyOfRow[i] = keycode;
					if(keycode != KeyCode::KEY_UNDEFINED){
						// Button Action
						displayQueue.offer(translateKeyCodeToLED(keycode));
					}
				}
			}
			lastKeyOfRow[i] = keycode;
			//Debounce End
			
			//
			// Do Display
			//

			HAL_GPIO_WritePin(GPIOB, 0xFF00, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, displayQueue[i], GPIO_PIN_SET);
			HAL_Delay(1);
			HAL_GPIO_WritePin(GPIOB, 0xFF00, GPIO_PIN_RESET);
		}
  }
  /* USER CODE END 3 */
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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
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
