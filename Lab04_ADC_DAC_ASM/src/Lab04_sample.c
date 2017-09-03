//--------------------------------
// Lab 4 - Sample - Lab04_sample.c
//--------------------------------
//
//

#include "Lab04_sample.h"

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	configureADC();

	// Code goes here
}

void configureADC()
{
	if (HAL_ADC_Init(&blah) != HAL_OK)
  {
    // Initialization error
    Error_Handler();
  }

	/* Available sampling times:

		ADC_SAMPLETIME_3CYCLES
	  ADC_SAMPLETIME_15CYCLES
		ADC_SAMPLETIME_28CYCLES
		ADC_SAMPLETIME_56CYCLES
		ADC_SAMPLETIME_84CYCLES
		ADC_SAMPLETIME_112CYCLES
		ADC_SAMPLETIME_144CYCLES
		ADC_SAMPLETIME_480CYCLES

	*/

	if(HAL_ADC_ConfigChannel(&blah, &blahconf) != HAL_OK)
	{
		Error_Handler();
	}
}


void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{

// GPIO init

}

/*
 * This is a simple error indicator, it blinks the red LED.
 */
void Error_Handler()
{
// Init LED1
	__HAL_RCC_GPIOJ_CLK_ENABLE(); // First things first: initialize the port clock!
	GPIOJ->MODER |= 0x04000000U; // Init LED1
	while(1)
	{
// Toggle LED1
		GPIOJ->ODR ^= GPIO_PIN_13;
		HAL_Delay(1000);
	}
}
