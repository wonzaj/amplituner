#include "SSD1322_HW_Driver.h"
#include "spi.h"

//====================== CS pin low ========================//
/**
 *  @brief Drives CS (Chip Select) pin of SPI interface low.
 *
 *  CS pin may be also signed as "SS" or "NSS"
 */
void SSD1322_HW_SetPin_CS_LOW()
{
	HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_RESET);
}

//====================== CS pin high ========================//
/**
 *  @brief Drives CS (Chip Select) pin of SPI interface high.
 *
 *  CS pin may be also signed as "SS" or "NSS"
 */
void SSD1322_HW_SetPin_CS_HIGH()
{
	HAL_GPIO_WritePin(SPI_CS_PORT, SPI_CS_PIN, GPIO_PIN_SET);
}

//====================== DC pin low ========================//
/**
 *  @brief Drives DC (Data/Command) pin of OLED driver low.
 *
 *  High state is for data and low state is for command.
 */
void SSD1322_HW_SetPin_DC_LOW()
{
	HAL_GPIO_WritePin(SPI_DC_PORT, SPI_DC_PIN, GPIO_PIN_RESET);
}

//====================== DC pin high ========================//
/**
 *  @brief Drives DC (Data/Command) pin of of OLED driver high.
 *
 *  High state is for data and low state is for command.
 */
void SSD1322_HW_SetPin_DC_HIGH()
{
	HAL_GPIO_WritePin(SPI_DC_PORT, SPI_DC_PIN, GPIO_PIN_SET);
}

//====================== RESET pin low ========================//
/**
 *  @brief Drives RESET pin of of OLED driver low.
 *
 *  Logic low on RESET resets OLED driver.
 */
void SSD1322_HW_SetPin_RESET_LOW()
{
	HAL_GPIO_WritePin(SPI_RESET_PORT, SPI_RESET_PIN, GPIO_PIN_RESET);
}

//====================== RESET pin high ========================//
/**
 *  @brief Drives RESET pin of of OLED driver high.
 *
 *  Logic low on RESET resets OLED driver.
 */
void SSD1322_HW_SetPin_RESET_HIGH()
{
	HAL_GPIO_WritePin(SPI_RESET_PORT, SPI_RESET_PIN, GPIO_PIN_SET);
}

//====================== Send single SPI byte ========================//
/**
 *  @brief Transmits single byte through SPI interface.
 *
 *  @param[in] byte_to_transmit byte that will be transmitted through SPI interface
 */
void SSD1322_HW_SPI_TX_Byte(uint8_t byte_to_transmit)
{
	HAL_SPI_Transmit(SPI_PERYPH, &byte_to_transmit, 1, 10);
}

//====================== Send array of SPI bytes ========================//
/**
 *  @brief Transmits array of bytes through SPI interface.
 *
 *  @param[in] array_to_transmit array of bytes that will be transmitted through SPI interface
 *  @param[in] array_size amount of bytes to transmit
 */
void SSD1322_HW_SPI_TX_Array(uint8_t *array_to_transmit, uint32_t array_size)
{
	HAL_SPI_Transmit(SPI_PERYPH, array_to_transmit, array_size,100);
}

//====================== Milliseconds delay ========================//
/**
 *  @brief Wait for x milliseconds.
 *
 *  NOTE: This function is only used in initialization sequence to pull reset down and high
 *  and later to wait for Vdd stabilization. It takes time ONLY on startup (around 200ms)
 *  and is NOT USED during normal operation.
 *
 *  @param[in] milliseconds time to wait
 */
void SSD1322_HW_msDelay(uint32_t milliseconds)
{
	HAL_Delay(milliseconds);
}
