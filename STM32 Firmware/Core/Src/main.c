/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD/LCD.h"
#include "Configuraciones.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
	HOME,
	SELECT_T1,
	SELECT_T2,
	SELECT_T3,
	EDIT_T1,
	EDIT_T2,
	EDIT_T3,
	EDIT_UNIT_T1,
	PRE_RUN,
	RUN,
	TRIGGERED,
	CONFIG,
	CONFIG_LOGIC_OUT1,
	CONFIG_LOGIC_OUT2,
	EDIT_CONFIG_LOGIC_OUT1,
	EDIT_CONFIG_LOGIC_OUT2,
	EXIT
} var_menu;

typedef enum {
	CMD_IDLE = -1, CMD_READING, CMD_READY
} var_cmd_it;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define BUTTON()( INV_LOGICA_SW ? HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) : !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) )
#define EXIT_MENU_DELAY HAL_Delay(200)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
void delay_us(uint32_t us);
void switchToMenu(var_menu new);
int8_t readEncoder();
int8_t readButton();

volatile var_menu menu = HOME;
type_data_t1 modeT1;
uint8_t logicaQ1 = 1, logicaQ2 = 1;
uint16_t T1, T2, T3;
uint16_t T1_lcd;

char buff_tx[50], buff_rx[50], buff_temp[10];
char charRead;
uint8_t buff_index;
volatile var_cmd_it cmdStatus = CMD_IDLE;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
	MX_TIM1_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	//Apago las interrupciones
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
	HAL_TIM_Base_Start(&htim1);

	//Inicializacion del LCD
	LCD_Init();
	LCD_setCursor(4, 0);
	LCD_printString("TEMPORIZADOR");
	LCD_setCursor(4, 1);
	LCD_printString("DE  DISPAROS");
	LCD_setCursor(5, 3);
	LCD_printString("-  LEME  -");
	HAL_Delay(2500);
	printHome(1);

	menu = HOME;
	int8_t encRead = readEncoder();
	T1_lcd = 10;
	T2 = 0;
	T3 = 0;
	modeT1 = T1_MS;

	HAL_UART_Receive_IT(&huart1, (uint8_t*) &charRead, 1);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		//************************************************************************/
		/* 			LEE LOS COMANDOS EN LA ESTRUCTURA CORRECTA         		     */
		/*************************************************************************/
		if (cmdStatus == CMD_READY) {
			//Deshabilito las interrupciones
			HAL_UART_AbortReceive_IT(&huart1);

			/*-------------------------------------------------------------------*/
			if (strncmp(buff_rx, "#01&", 4) == 0) {
				sprintf(buff_tx, "#OK&");
				HAL_UART_Transmit_IT(&huart1, (uint8_t*) buff_tx, strlen(buff_tx));
			} else if (strncmp(buff_rx, "#10&", 4) == 0) {							//#10& (Devuelvo los tiempos)
				sprintf(buff_tx, "#10$%05d%05d%05d&", T1_lcd, T2, T3);
				HAL_UART_Transmit_IT(&huart1, (uint8_t*) buff_tx, strlen(buff_tx));
			} else if (strncmp(buff_rx, "#11$", 4) == 0 && strlen(buff_rx) == 20) {	//$11& (Recibo los tiempos de la app)
				//T1
				strncpy(buff_temp, buff_rx + 4, 5);
				buff_temp[5] = '\0';
				sscanf(buff_temp, "%hd", &T1_lcd);
				//T2
				strncpy(buff_temp, buff_rx + 9, 5);
				buff_temp[5] = '\0';
				sscanf(buff_temp, "%hd", &T2);
				//T3
				strncpy(buff_temp, buff_rx + 14, 5);
				buff_temp[5] = '\0';
				sscanf(buff_temp, "%hd", &T3);

				//Respuesta
				sprintf(buff_tx, "#11&");
				HAL_UART_Transmit_IT(&huart1, (uint8_t*) buff_tx, strlen(buff_tx));

				printNewValuesBT(1);
				HAL_Delay(2000);
				switchToMenu(menu);
			} else if (strncmp(buff_rx, "#20&", 4) == 0) {
				OUT1_ON(logicaQ1);
				sprintf(buff_tx, "#20&");
				HAL_UART_Transmit_IT(&huart1, (uint8_t*) buff_tx, strlen(buff_tx));
			} else if (strncmp(buff_rx, "#21&", 4) == 0) {
				OUT1_OFF(logicaQ1);
				sprintf(buff_tx, "#21&");
				HAL_UART_Transmit_IT(&huart1, (uint8_t*) buff_tx, strlen(buff_tx));
			} else if (strncmp(buff_rx, "#22&", 4) == 0) {
				OUT2_ON(logicaQ2);
				sprintf(buff_tx, "#22&");
				HAL_UART_Transmit_IT(&huart1, (uint8_t*) buff_tx, strlen(buff_tx));
			} else if (strncmp(buff_rx, "#23&", 4) == 0) {
				OUT2_OFF(logicaQ2);
				sprintf(buff_tx, "#23&");
				HAL_UART_Transmit_IT(&huart1, (uint8_t*) buff_tx, strlen(buff_tx));
			} else if (strncmp(buff_rx, "#50&", 4) == 0) {
				sprintf(buff_tx, "#50&");
				HAL_UART_Transmit_IT(&huart1, (uint8_t*) buff_tx, strlen(buff_tx));

				OUT1_OFF(logicaQ1);
				OUT2_OFF(logicaQ2);
				switchToMenu(RUN);

				/************************************************************************/
				/*                      PREPARACION PARA DISPARO                        */
				/************************************************************************/
				T1 = T1_lcd;
				if (T1_lcd < MIN_T1_TO_ADD_20MS)
					T1 += 20000;

				__HAL_GPIO_EXTI_CLEAR_IT(SIG_INPUT_Pin); //Limpio el flag de la interrupcion
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);	//Habilito la interrupcion
				/************************************************************************/
			}


			/*-------------------------------------------------------------------*/
			cmdStatus = CMD_IDLE;
			//Habilito nuevamente las interrupciones
			HAL_UART_Receive_IT(&huart1, (uint8_t*) &charRead, 1);
		}
		/*************************************************************************/

		encRead = readEncoder();
		//HAL_Delay(1);

		switch (menu) {
		case HOME: //============================================================================== HOME
			if (readButton() == 1) {
				switchToMenu(SELECT_T1);
			}
			break;

		case SELECT_T1: //========================================================================= SELECT_T1
			if (encRead == 1) {
				switchToMenu(SELECT_T2);
			}

			if (readButton() == 1) {
				//Si las unidades son Grados o ms, entonces convierto el actual (que siempre estara en us)
				//a la unidad correspondiente.
				if (modeT1 == T1_GRADOS)
					T1_lcd = (uint16_t) (T1_lcd * 360.0 / 20000.0);
				else if (modeT1 == T1_MS)
					T1_lcd = (uint16_t) (T1_lcd / 1000.0);

				switchToMenu(EDIT_T1);
			}
			break;

		case EDIT_T1: //========================================================================== EDIT_T1
			if (encRead != 0) {
				//Si giro el encoder, incrementa o decrementa segun la unidad.
				//Finalmente, actualizo el LCD.
				switch (modeT1) {
				case T1_US:
					T1_lcd += encRead * MIN_SALTO_T1;
					if (T1_lcd < 0)
						T1_lcd = 0;
					if (T1_lcd > MAX_T1_US)
						T1_lcd = MAX_T1_US;
					break;
				case T1_MS:
					T1_lcd += encRead;
					if (T1_lcd < 0)
						T1_lcd = 0;
					if (T1_lcd > MAX_T1_MS)
						T1_lcd = MAX_T1_MS;
					break;
				case T1_GRADOS:
					T1_lcd += encRead;
					if (T1_lcd < 0)
						T1_lcd = 0;
					if (T1_lcd > MAX_T1_GRADOS)
						T1_lcd = MAX_T1_GRADOS;
					break;
				}
				printEditT1(1, T1_lcd, modeT1);
			}

			switch (readButton()) {
			case 1:
				switch (modeT1) {

				case T1_GRADOS:
					//Al hacer una pulsacion corta, vuelvo al menu de SELECT_T1 pero antes
					//convierto de nuevo a US segun la unidad actual.
					T1_lcd = (uint16_t) (T1_lcd * 20000.0 / 360.0);
					break;
				case T1_MS:
					T1_lcd = (uint16_t) (T1_lcd * 1000.0);
					break;
				default:
					break;
				}
				switchToMenu(SELECT_T1);
				break;
			case -1:
				//Si realizo una pulsacion larga, entonces voy directo al menu de edicion de unidad
				switchToMenu(EDIT_UNIT_T1);
				break;
			default:
				break;
			}
			break;

		case SELECT_T2: //======================================================================== SELECT_T2
			if (encRead == 1) {
				switchToMenu(SELECT_T3);
			} else if (encRead == -1) {
				switchToMenu(SELECT_T1);
			}

			if (readButton() == 1) {
				switchToMenu(EDIT_T2);
			}
			break;

		case EDIT_T2: //========================================================================== EDIT_T2
			if (encRead != 0) {
				T2 += encRead * MIN_SALTO_T2;
				if (T2 < MIN_SALTO_T2)
					T2 = MIN_SALTO_T2;
				if (T2 > MAX_T2)
					T2 = MAX_T2;
				printEditT2(0, T2);
			}

			if (readButton()) {
				switchToMenu(SELECT_T2);
			}
			break;

		case EDIT_UNIT_T1: //===================================================================== EDIT_UNIT_T1
			//Segun el sentido de giro y la unidad actual, cambio a la nueva unidad
			//pero ademas, actualizo el valor actual a la nueva unidad.
			if (encRead == 1) {
				if (modeT1 == T1_US) { //[us]  -->  [ms]
					modeT1 = T1_MS;
					T1_lcd = (uint16_t) (T1_lcd / 1000.0);
				} else if (modeT1 == T1_MS) {  //[ms]  -->  [grados]
					modeT1 = T1_GRADOS;
					T1_lcd = (uint16_t) (T1_lcd * 360.0 / 20.0);
				}
				printEditT1Unit(1, T1_lcd, modeT1);
			} else if (encRead == -1) {
				if (modeT1 == T1_GRADOS) {  //[grados]  -->  [ms]
					modeT1 = T1_MS;
					T1_lcd = (uint16_t) (T1_lcd * 20.0 / 360.0);
				} else if (modeT1 == T1_MS) {  //[ms]  -->  [us]
					modeT1 = T1_US;
					T1_lcd = (uint16_t) (T1_lcd * 1000.0);
				}
				printEditT1Unit(1, T1_lcd, modeT1);
			}

			if (readButton() == -1) {
				switchToMenu(EDIT_T1);
			}
			break;

		case SELECT_T3: //======================================================================== SELECT_T3
			if (encRead == 1) {
				switchToMenu(PRE_RUN);
			} else if (encRead == -1) {
				switchToMenu(SELECT_T2);
			}

			if (readButton()) {
				switchToMenu(EDIT_T3);
			}
			break;

		case EDIT_T3: //========================================================================== EDIT_T3
			if (encRead != 0) {
				T3 += encRead * MIN_SALTO_T3;
				if (T3 < MIN_SALTO_T3)
					T3 = MIN_SALTO_T3;
				if (T3 > MAX_T3)
					T3 = MAX_T3;
				printEditT3(1, T3);
			}

			if (readButton()) {
				switchToMenu(SELECT_T3);
			}
			break;

		case PRE_RUN: //========================================================================== PRE_RUN (Iniciar?)
			if (encRead == -1) {
				switchToMenu(SELECT_T3);
			} else if (encRead == 1) {
				switchToMenu(CONFIG);
			}

			if (readButton() == -1) {
				OUT1_OFF(logicaQ1);
				OUT2_OFF(logicaQ2);
				switchToMenu(RUN);

				/************************************************************************/
				/*                      PREPARACION PARA DISPARO                        */
				/************************************************************************/
				T1 = T1_lcd;
				if (T1_lcd < MIN_T1_TO_ADD_20MS)
					T1 += 20000;

				__HAL_GPIO_EXTI_CLEAR_IT(SIG_INPUT_Pin); //Limpio el flag de la interrupcion
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);	//Habilito la interrupcion
				/************************************************************************/
			}
			break;

		case TRIGGERED: //======================================================================== TRIGGERED
			printTriggered();

			uint8_t fin = 0;
			while (!readButton() && fin != 1) {
				HAL_Delay(1);

				//Verifico si recibo el comando para finalizar por Bluetooth
				if (cmdStatus == CMD_READY) {
					//Deshabilito las interrupciones
					HAL_UART_AbortReceive_IT(&huart1);

					if (strncmp(buff_rx, "#51&", 4) == 0) {
						sprintf(buff_tx, "#51&");
						HAL_UART_Transmit_IT(&huart1, (uint8_t*) buff_tx, strlen(buff_tx));
						fin = 1;
					}

					cmdStatus = CMD_IDLE;
					//Habilito nuevamente las interrupciones
					HAL_UART_Receive_IT(&huart1, (uint8_t*) &charRead, 1);
				}
			}

			OUT1_OFF(logicaQ1);
			OUT2_OFF(logicaQ2);
			switchToMenu(HOME);
			break;

		case CONFIG: //=========================================================================== CONFIG (Entrar?)
			if (encRead == -1) {
				switchToMenu(PRE_RUN);
			}

			if (readButton()) {
				switchToMenu(CONFIG_LOGIC_OUT1);
			}
			break;

		case CONFIG_LOGIC_OUT1: //================================================================ CONFIG_LOGIC_OUT1
			if (encRead == 1) {
				switchToMenu(CONFIG_LOGIC_OUT2);
			}

			if (readButton()) {
				switchToMenu(EDIT_CONFIG_LOGIC_OUT1);
			}
			break;

		case EDIT_CONFIG_LOGIC_OUT1: //=========================================================== EDIT_CONFIG_LOGIC_OUT1
			if (logicaQ1 && encRead == -1) {
				logicaQ1 = 0;
				printEditConfigLogT1(1, logicaQ1);
			} else if (!logicaQ1 && encRead == 1) {
				logicaQ1 = 1;
				printEditConfigLogT1(1, logicaQ1);
			}

			if (readButton()) {
				OUT1_OFF(logicaQ1);
				switchToMenu(CONFIG_LOGIC_OUT1);
			}
			break;

		case CONFIG_LOGIC_OUT2: //================================================================ CONFIG_LOGIC_T2
			if (encRead == -1) {
				switchToMenu(CONFIG_LOGIC_OUT1);
			} else if (encRead == 1) {
				switchToMenu(EXIT);
			}

			if (readButton()) {
				switchToMenu(EDIT_CONFIG_LOGIC_OUT2);
			}
			break;

		case EDIT_CONFIG_LOGIC_OUT2: //=========================================================== EDIT_CONFIG_LOGIC_T2
			if (logicaQ2 && encRead == -1) {
				logicaQ2 = 0;
				printEditConfigLogT2(1, logicaQ2);
			} else if (!logicaQ2 && encRead == 1) {
				logicaQ2 = 1;
				printEditConfigLogT2(1, logicaQ2);
			}

			if (readButton()) {
				OUT2_OFF(logicaQ2);
				switchToMenu(CONFIG_LOGIC_OUT2);
			}
			break;

		case EXIT:				//EXIT
			if (encRead == -1) {
				switchToMenu(CONFIG_LOGIC_OUT2);
			}

			if (readButton()) {
				switchToMenu(CONFIG);
			}
			break;
		default:
			break;

		}
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 72 - 1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 0xffff - 1;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */

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
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA,
	LCD_RS_Pin | LCD_E_Pin | LCD_DB4_Pin | LCD_DB5_Pin | LCD_DB6_Pin | LCD_DB7_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, OUT_T1_Pin | OUT_T2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : LCD_RS_Pin LCD_E_Pin LCD_DB4_Pin LCD_DB5_Pin
	 LCD_DB6_Pin LCD_DB7_Pin */
	GPIO_InitStruct.Pin = LCD_RS_Pin | LCD_E_Pin | LCD_DB4_Pin | LCD_DB5_Pin | LCD_DB6_Pin | LCD_DB7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : ENC_SW_Pin */
	GPIO_InitStruct.Pin = ENC_SW_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(ENC_SW_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : ENC_CLK_Pin ENC_DT_Pin */
	GPIO_InitStruct.Pin = ENC_CLK_Pin | ENC_DT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : SIG_INPUT_Pin */
	GPIO_InitStruct.Pin = SIG_INPUT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SIG_INPUT_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : OUT_T1_Pin OUT_T2_Pin */
	GPIO_InitStruct.Pin = OUT_T1_Pin | OUT_T2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/************************************************************************/
/*       	INTERRUPCION PARA RECEPCION DE DATOS POR USART		        */
/************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (charRead == '#' && (cmdStatus == CMD_IDLE || cmdStatus == CMD_READING)) {
		buff_index = 0;
		buff_rx[0] = charRead;
		buff_index++;
		cmdStatus = CMD_READING;
	} else if (charRead == '&' && cmdStatus == CMD_READING) {
		buff_rx[buff_index] = charRead;
		buff_rx[buff_index + 1] = '\0';
		cmdStatus = CMD_READY;
	} else if (cmdStatus == CMD_READING) {
		buff_rx[buff_index] = charRead;
		buff_index++;
	}

	HAL_UART_Receive_IT(&huart1, (uint8_t*) &charRead, 1);
}

/************************************************************************/
/*     					INTERRUPCION DEL CRUCE POR CERO			        */
/************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	//Quito el flag y apago la interrupcion.
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

	if (T1 != 0) {
		while (__HAL_TIM_GET_COUNTER(&htim1) < T1)
			;
	}

	OUT1_ON(logicaQ1);

	if (T3 == 0) {
		HAL_Delay(T2);
		OUT1_OFF(logicaQ1);
	} else if (T3 < T2) {
		HAL_Delay(T3);
		OUT2_ON(logicaQ2);
		HAL_Delay(T2 - T3);
		OUT1_OFF(logicaQ1);
	} else if (T3 > T2) {
		HAL_Delay(T2);
		OUT1_OFF(logicaQ1);
		HAL_Delay(T3 - T2);
		OUT2_ON(logicaQ2);
	} else {
		HAL_Delay(T2);
		OUT1_OFF(logicaQ1);
		OUT2_ON(logicaQ2);
	}

	menu = TRIGGERED;
}

/************************************************************************/
/*     							DELAY EN US          					*/
/************************************************************************/
void delay_us(uint32_t us) {
	__HAL_TIM_SET_COUNTER(&htim1, 0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < us)
		;  // wait for the counter to reach the us input in the parameter
}

/************************************************************************/
/*     					PERMITE CAMBIAR ENTRE LOS MENUS          		*/
/************************************************************************/
void switchToMenu(var_menu new) {
	menu = new;
	switch (new) {
	case HOME:
		printHome(1);
		break;
	case SELECT_T1:
		printViewT1(1, T1_lcd, modeT1);
		break;
	case SELECT_T2:
		printViewT2(1, T2);
		break;
	case SELECT_T3:
		printViewT3(1, T3);
		break;
	case EDIT_T1:
		printEditT1(1, T1_lcd, modeT1);
		break;
	case EDIT_T2:
		printEditT2(1, T2);
		break;
	case EDIT_T3:
		printEditT3(1, T3);
		break;
	case EDIT_UNIT_T1:
		printEditT1Unit(1, T1_lcd, modeT1);
		break;
	case PRE_RUN:
		printPreRun(1, T1_lcd, T2, T3);
		break;
	case RUN:
		printRunning(T1_lcd, T2, T3);
		break;
	case TRIGGERED:
		printTriggered();
		break;
	case CONFIG:
		printConfigEntrar(1);
		break;
	case CONFIG_LOGIC_OUT1:
		printViewConfigLogT1(1, logicaQ1);
		break;
	case EDIT_CONFIG_LOGIC_OUT1:
		printEditConfigLogT1(1, logicaQ1);
		break;
	case CONFIG_LOGIC_OUT2:
		printViewConfigLogT2(1, logicaQ2);
		break;
	case EDIT_CONFIG_LOGIC_OUT2:
		printEditConfigLogT2(1, logicaQ2);
		break;
	case EXIT:
		printConfigVolver(1);
		break;
	}
}

/************************************************************************/
/*     							ENCODER ROTATIVO          				*/
/************************************************************************/
int8_t readEncoder() {
	int8_t result = 0;
	static uint8_t prev_CLK = 0;

	uint8_t now_CLK = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
	uint8_t now_DT = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);

	if (prev_CLK != now_CLK) {
		HAL_Delay(1);
		prev_CLK = now_CLK;

		if (now_CLK == 0) {
			if (now_DT == 0) {
				result = INV_LOGICA_ENCODER ? 1 : -1;
			} else if (now_DT == 1) {
				result = INV_LOGICA_ENCODER ? -1 : 1;
			}
		}
	}

	return (result);
}

/************************************************************************/
/*     				PULSADOR DEL ENCODER ROTATIVO          				*/
/*	Devuelve:															*/
/* 		-1: Pulsacion larga												*/
/*		 0: No se presiono												*/
/*		 1: Presion corta												*/
/************************************************************************/
int8_t readButton() {
	static uint8_t bt_prev = 0;
	uint8_t bt_now = BUTTON();
	uint8_t result = 0;
	uint16_t dt = 0;

	if (bt_now != bt_prev) {
		if (bt_now == 1 && bt_prev == 0) {
			HAL_Delay(5);
			while (BUTTON() == 1) {
				HAL_Delay(1);
				dt++;
				if (dt > MIN_MS_ENCODER_LT) {
					result = -1;
					break;
				} else if (dt < MAX_MS_ENCODER_ST) {
					result = 1;
				}
			}
		}

		bt_prev = bt_now;
	}

	return (result);
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
