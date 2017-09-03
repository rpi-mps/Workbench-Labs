//----------------------------------
// Lab 2 - Timer Interrupts - Lab02.c
//----------------------------------
// Objective:
//   Build a small game that records user's reaction time.
//

//
//
// -- Imports ---------------
//
#include "Lab02_intrpt.h"

//
//
// -- Code Body -------------
//
volatile uint8_t timeUpdated = 0;
volatile uint8_t buttonPressed = 0;
volatile uint8_t buttonReleased = 0;
volatile uint32_t elapsed = 0;

int32_t randomNumber = 0;
uint32_t startTime = 0;
float averageScore = 0;
unsigned int iterations = 0;

int main() {
	Sys_Init();
	Init_Timer();
	Init_GPIO();

	// Setup random number generation.
	// srand(0);

	while (1) {
		// Main loop code goes here
		printf("\033c\033[36m\033[2J");
		printf("Blink!\r\n");
		HAL_Delay(1000);
		blinkScreen();
		HAL_Delay(1000);
	}
}

//
//
// -- Utility Functions ------
//
void blinkScreen(){
	printf("\033[30;47m");
	// Clear and redraw display (flash it & sound the bell).
	printf("\a\033[s\033[2J\033[u");
	fflush(stdout);
	HAL_Delay(100);

	printf("\033[37;40m");
	// Clear and redraw display (flash it).
	printf("\033[s\033[2J\033[u");
	fflush(stdout);
}


//
//
// -- Init Functions ----------
//
void Init_Timer() {
	// Enable the TIM6 interrupt.
	// Looks like HAL hid this little gem, this register isn't mentioned in
	//   the STM32F7 ARM Reference Manual....
	// NVIC->ISER

	// Enable TIM6 clock
	// RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	// asm ( "nop" );
	// asm ( "nop" );

	// Set pre-scaler to make a 100kHz ticker.
	// TIM6->PSC

	// Set the Auto-reload Value for 10Hz overflow
	// TIM6->ARR

	// Generate update events to auto reload.
	// TIM6->EGR

	// Enable Update Interrupts.
	// TIM6->DIER

	// Start the timer.
	// TIM6->CR1
}

void Init_GPIO() {
	// Enable GPIO clocks?
	// Looks like GPIO reg updates are synced to a base clock.
	//  for any changes to appear the clocks need to be running.
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
	// Delay after an RCC peripheral clock enabling
	asm ("nop");
	asm ("nop");

	// Set Pin 13/5 to output. (LED1 and LED2)
	//GPIOJ->MODER

	// GPIO Interrupt
	// By default pin 0 will trigger the interrupt,
	//  so no need to mess with SYSCFG_EXTICR1.

	// Set Pin 0 as input (button) with pull-down.
	//GPIOA->PUPDR

	// Set interrupt enable for EXTI0.
	// NVIC->ISER

	// Unmask interrupt.
	// EXTI->IMR

	// Register for rising edge.
	// EXTI->RTSR

	// And register for the falling edge.
	// EXTI->FTSR
}

//
//
// -- ISRs (IRQs) -------------
//
void TIM6_DAC_IRQHandler() {
	// Clear Interrupt Bit
	TIM6->SR = ~TIM_DIER_UIE;

	// Other code here:

}

void EXTI0_IRQHandler() {
	// Clear Interrupt Bit (by setting it, weird I know).
	EXTI->PR |= EXTI_PR_PR0;

	// Other code here:

}
