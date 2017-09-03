//----------------------------------
// Lab 2 - Timer Interrupts - Lab02.h
//----------------------------------

#ifndef __Lab02_intrpt_H
#define __Lab02_intrpt_H

//#include <stdlib.h>
#include "stm32f769xx.h"
#include "stm32f7xx_hal.h"
#include "init.h"

//
//
// -- Function Defines ------
//
void Init_Timer();
void Init_GPIO();

void blinkScreen();

void TIM6_DAC_IRQHandler();
void EXTI0_IRQHandler();

#endif /* __Lab02_intrpt_H */
