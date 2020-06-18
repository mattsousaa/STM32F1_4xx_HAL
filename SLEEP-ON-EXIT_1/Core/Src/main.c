#include <string.h>
#include "main.h"
#include "stm32f1xx_hal_tim.h"

void SystemClockConfig(void);
void MX_USART2_UART_Init(void);
void TIMER3_Init(void);
void GPIO_Init(void);
void Error_Handler(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

/* TIMER3 can be used for basic timer
 * See STM32 cross-series timer overview document for more details */
TIM_HandleTypeDef htimer3;
UART_HandleTypeDef huart2;

extern uint8_t some_data[];

int main(void){

	HAL_Init();
	SystemClockConfig();
	GPIO_Init();
	MX_USART2_UART_Init();

	TIMER3_Init();

	//SCB->SCR |= (1 << 1); // See on Generic user guide
	/* Current on run mode: 4.5 mA
	 * Current on sleep mode: 3.10 mA */
	HAL_PWR_EnableSleepOnExit(); // Enter sleep mode, on return from an ISR.

	/* lets start with fresh Status register of Timer to avoid any spurious interrupts */
	TIM3->SR = 0;

	//Lets start timer in IT mode
	HAL_TIM_Base_Start_IT(&htimer3);

	while(1);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(HAL_UART_Transmit(&huart2, (uint8_t*)some_data, (uint16_t)strlen((char*)some_data), HAL_MAX_DELAY) != HAL_OK){
		Error_Handler();
	}
}

void SystemClockConfig(void){


}

void GPIO_Init(void){

	GPIO_InitTypeDef ledgpio;

	/* GPIOA clock enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* High Level GPIO Initialization */
	ledgpio.Pin = GPIO_PIN_1;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	ledgpio.Speed = GPIO_SPEED_FREQ_LOW;

	/* Init GPIO */
	HAL_GPIO_Init(GPIOA, &ledgpio);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
}

void TIMER3_Init(void){

	/* Verify the spreadsheet "timer_period_calculation" to adjust Prescaler and Period values;
	 * Prescaler value is stored in TIMx_PSC register (16 bits);
	 * Period value is stored in TIMx_ARR register (16 bits - MAX 65535);
	 * Choose a clock value for your microcontroller in the spreadsheet (TIMx_CLK);
	 * Choose a time base required in seconds in the spreadsheet;
	 * Check if the period exceeded the maximum value of TIMx_ARR register (16 bits - MAX 65535);
	 * If yes, increase or decrease the prescaler in the spreedsheet until 0 < TIMx_ARR <= 65535;
	 * This process will find how many ticks there exists for every period of clock;
	 * For instance, if TIMx_CLK = 16MHz and prescaler = 0, so for every 0.0625us a tick happens;
	 * In this case, what is the period value must be configured to get the time base of 100ms?;
	 * This math results in 1600000 and should be placed in TIMx_ARR register, but this value is greater than 65535;
	 * That's why you should increase or decrease the prescaler value.
	 * */

	/* Create a time base for 10ms with SYSCLK = 8MHz */
	htimer3.Instance = TIM3;
	htimer3.Init.Prescaler = 9;
	htimer3.Init.Period = 8000-1;	// The update event happens after one time gap or one time period

	if(HAL_TIM_Base_Init(&htimer3) != HAL_OK){
		Error_Handler();
	}

}

void MX_USART2_UART_Init(void){

	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX;

	if(HAL_UART_Init(&huart2) != HAL_OK){
		Error_Handler();
  	}

}

void Error_Handler(void){
	while(1);
}
