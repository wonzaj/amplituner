#ifndef SSD1322_HW_DRIVER_H
#define SSD1322_HW_DRIVER_H

#include "main.h"

#define SPI_PERYPH &hspi4

#define SPI_DC_PORT 		DISPLAY_SPI_DC_GPIO_Port
#define SPI_DC_PIN 		DISPLAY_SPI_DC_Pin
#define SPI_RESET_PORT	DISPLAY_SPI_RESET_GPIO_Port
#define SPI_RESET_PIN		DISPLAY_SPI_RESET_Pin
#define SPI_CS_PORT		DISPLAY_SPI_CS_GPIO_Port
#define SPI_CS_PIN		DISPLAY_SPI_CS_Pin

void SSD1322_HW_SetPin_CS_LOW();
void SSD1322_HW_SetPin_CS_HIGH();
void SSD1322_HW_SetPin_DC_LOW();
void SSD1322_HW_SetPin_DC_HIGH();
void SSD1322_HW_SetPin_RESET_LOW();
void SSD1322_HW_SetPin_RESET_HIGH();
void SSD1322_HW_SPI_TX_Byte(uint8_t byte_to_transmit);
void SSD1322_HW_SPI_TX_Array(uint8_t *array_to_transmit, uint32_t array_size);
void SSD1322_HW_msDelay(uint32_t milliseconds);

#endif /* SSD1322_HW_DRIVER_H */
