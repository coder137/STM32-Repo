/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

// Contains the _write and _read functions (_weak can override)
// Already included by the stdio.h library, (Kept here to view the source code)
//#include <unistd.h>

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
SPI_HandleTypeDef hspi3;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI3_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

static void send_spi_tx(uint8_t *data, int size);
static void send_spi_dummy_tx(uint8_t *tx, int16_t size);

static void ism_activate();
static void ism_deactivate();

static int ism_read_status();

static void ism_read();
static void ism_write();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static uint8_t ACK[2] = { 0x0A, 0x0A };

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_SPI3_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	printf("Starting USER Code\r\n");

	HAL_StatusTypeDef status;
//	static uint8_t rData[512];

// TODO, Reset Module
	printf("Resetting Module\r\n");
	HAL_GPIO_WritePin(ISM43362_RST_GPIO_Port, ISM43362_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);

	HAL_GPIO_WritePin(ISM43362_RST_GPIO_Port, ISM43362_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(500);

	// TODO, Set the Module Active
	printf("Setting Module active\r\n");
//	HAL_GPIO_WritePin(ISM43362_SPI3_CSN_GPIO_Port, ISM43362_SPI3_CSN_Pin,
//			GPIO_PIN_RESET);

	/**
	 *
	 * START
	 *
	 */

	int s;
	while((s = ism_read_status()) == 0) {
		printf("%d.\r\n", s);
		HAL_Delay(1000);
	}

	ism_activate();
	ism_read();
	ism_deactivate();

	// TODO, Might need a delay here
	__NOP();
	__NOP();

	ism_activate();
	uint8_t d[] = "I?\r\n";
	send_spi_tx(d, 4);
	ism_deactivate();

//	printf("ism_read_status: %d\r\n", ism_read_status());
//	__NOP();
//	__NOP();

	while((s = ism_read_status()) == 0) {
		printf("%d.\r\n", s);
		HAL_Delay(1000);
	}
	printf("%d\r\n", ism_read_status());

	ism_activate();
	ism_read();
	ism_deactivate();

	return 0;



	// 0x0d 0x0a 0x3e 0x20
	// 0x0a 0x0d 0x20 0x3e
	while (1) {

		ism_activate();
		printf("State: %d\r\n", ism_read_status());

		uint8_t t[2];
		int count = 0;

		while (ism_read_status() != 1) {
			HAL_SPI_Transmit(&hspi3, ACK, 2, 1000);
			printf(".\r\n");
			HAL_Delay(1000);

			count++;
			if (count == 10) {
				for (int i = 0; i < 20; i++) {
					status = HAL_SPI_Receive_IT(&hspi3, t, 2);
					printf("S %d D %x %x\r\n", status, t[1], t[0]);

				}
				count = 0;
			}
		}

		HAL_Delay(1000);
		ism_read();

		ism_deactivate();

//		send_spi_tx("?\r\n\n");

		GPIO_PinState state = HAL_GPIO_ReadPin(ISM43362_DRDY_EXTI1_GPIO_Port,
		ISM43362_DRDY_EXTI1_Pin);
		while (state != 1) {
			HAL_SPI_Transmit(&hspi3, ACK, 2, 1000);
			state = HAL_GPIO_ReadPin(ISM43362_DRDY_EXTI1_GPIO_Port,
			ISM43362_DRDY_EXTI1_Pin);
			printf("%d.\r\n", state);
			HAL_Delay(1000);

			count++;
			if (count == 10) {
				for (int i = 0; i < 20; i++) {
					status = HAL_SPI_Receive_IT(&hspi3, t, 2);
					printf("S %d D %x %x\r\n", status, t[1], t[0]);

				}
				count = 0;
			}
		}
//		uint8_t data[] = { 0x3F, 0x49, 0x0A, 0x0D, 0x0A, 0x0A };
//		send_spi_dummy_tx(data, 6);
	}

//
//	memset(rData, 0, 512 * sizeof(uint8_t));

	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		printf("Hello World\r\n");
		HAL_Delay(1000);
	}
	/* USER CODE END 3 */
}

static void send_spi_dummy_tx(uint8_t *tx, int16_t size) {
	(void) HAL_SPI_Transmit(&hspi3, tx, size, 1000);
	for (int i = 0; i < size; i = i + 2) {
		printf("Sent: %x %x\r\n", tx[i + 1], tx[i]);
	}
//	printf("S: %d, Sent: %s\r\n", status, tx);
	printf("DONE\r\n");
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 40;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief SPI3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI3_Init(void) {

	/* USER CODE BEGIN SPI3_Init 0 */

	/* USER CODE END SPI3_Init 0 */

	/* USER CODE BEGIN SPI3_Init 1 */

	/* USER CODE END SPI3_Init 1 */
	/* SPI3 parameter configuration*/
	hspi3.Instance = SPI3;
	hspi3.Init.Mode = SPI_MODE_MASTER;
	hspi3.Init.Direction = SPI_DIRECTION_2LINES;
	hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi3.Init.NSS = SPI_NSS_SOFT;
	hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi3.Init.CRCPolynomial = 7;
	hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	if (HAL_SPI_Init(&hspi3) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI3_Init 2 */

	/* USER CODE END SPI3_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE, ISM43362_RST_Pin | ISM43362_SPI3_CSN_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, ISM43362_BOOT0_Pin | ISM43362_WAKEUP_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pins : ISM43362_RST_Pin ISM43362_SPI3_CSN_Pin */
	GPIO_InitStruct.Pin = ISM43362_RST_Pin | ISM43362_SPI3_CSN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : ISM43362_BOOT0_Pin ISM43362_WAKEUP_Pin */
	GPIO_InitStruct.Pin = ISM43362_BOOT0_Pin | ISM43362_WAKEUP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : ISM43362_DRDY_EXTI1_Pin */
	GPIO_InitStruct.Pin = ISM43362_DRDY_EXTI1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ISM43362_DRDY_EXTI1_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

static void ism_read() {
	uint8_t t[2] = {0};
	while (ism_read_status()) {
		HAL_StatusTypeDef status = HAL_SPI_Receive_IT(&hspi3, t, 2);
		printf("S %d\r\n", status);
		printf("%x %x %c : %c\r\n", t[1], t[0], t[1], t[0]);
//		HAL_Delay(500);
	}
}

static void ism_write() {

}

static void ism_activate() {
	HAL_GPIO_WritePin(ISM43362_SPI3_CSN_GPIO_Port, ISM43362_SPI3_CSN_Pin,
			GPIO_PIN_RESET);
}

static void ism_deactivate() {
	HAL_GPIO_WritePin(ISM43362_SPI3_CSN_GPIO_Port, ISM43362_SPI3_CSN_Pin,
			GPIO_PIN_SET);
}

static int ism_read_status() {
	return HAL_GPIO_ReadPin(ISM43362_DRDY_EXTI1_GPIO_Port,
	ISM43362_DRDY_EXTI1_Pin);
}

static void send_spi_tx(uint8_t *data, int size) {
	for (int i = 0; i < size; i = i + 2) {
		uint8_t n[2] = { data[i + 1], data[i] };
		HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi3, n, 2, 1000);
		printf("Sent: d %d w %x %x\r\n", status, n[0], n[1]);
	}

//	HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi3, (uint8_t*) data, len,
//			1000);
//	printf("Sent: d %d w %s l %d\r\n", status, data, len);
}

/**
 * IMP, Make sure you return the length of the transfered data
 * Failing to do so will send the bytes that have NOT been sent (continuously)
 */
int _write(int fd, void *buf, size_t nbyte) {
	HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, (uint8_t*) buf, nbyte,
			1000);
	return (status == HAL_OK) ? nbyte : 0;
}

/**
 * NOTE, This fails when count = 0 (returns -1 and scanf has strange behaviour -> TODO)
 * To replicate the strange behaviour,
 * - change UINT32_MAX to a smaller value, i.e 10000 (10 seconds) and let it timeout.
 * - Uncomment the printf debugging line `Count ret`
 * - Check the return value of scanf (will be -1)
 * - Scanf stops being blocked because of this behaviour and keeps returning -1
 */
int _read(int fd, void *buf, size_t nbyte) {
	uint8_t d = 0;
	size_t count = 0;

	uint8_t *ptr = (uint8_t*) buf;
	HAL_StatusTypeDef status = HAL_UART_Receive(&huart1, &d,
			1 * sizeof(uint8_t),
			UINT32_MAX);
	if (status == HAL_OK && d != 0) {
		ptr[0] = d;
		putchar(d); // NOTE, Put this back onto the console
		count++;
	}

//	printf("Count ret: %d\r\n", count);
	return count;
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
