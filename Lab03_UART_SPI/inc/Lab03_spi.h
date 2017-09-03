//-----------------------------------
// Lab 3 - Part 3: SPI - Lab03_spi.h
//-----------------------------------
//

#ifndef __Lab03_spi_H
#define __Lab03_spi_H

#include "stm32f769xx.h"
#include "stm32f7xx_hal.h"
#include "init.h"

// For convenience only
#define SPI_SCK_PIN GPIO_PIN_xx  // ARD_xxx
#define SPI_SCK_PORT GPIOx
#define SPI_MISO_PIN GPIO_PIN_xx // ARD_xxx
#define SPI_MISO_PORT GPIOx
#define SPI_MOSI_PIN GPIO_PIN_xx // ARD_xxx
#define SPI_MOSI_PORT GPIOx
#define SPI_NSS_PIN GPIO_PIN_xx // ARD_xxx
#define SPI_NSS_PORT GPIOx

void configureSPI(void);

// Header code...

/*
// Some random SPI2 interrupt-related function names.
// Some may find these useful.
// See 769 Description of HAL drivers.pdf, Ch. 58.2.3 or in stm32f7xx_hal_spi.c/.h

void SPI2_IRQHandler(void);
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);
*/
#endif /* __Lab03_spi_H */
