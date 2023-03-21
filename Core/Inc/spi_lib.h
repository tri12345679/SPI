/*
 * spi_lib.h
 *
 *  Created on: Jan 5, 2023
 *      Author: Admin
 */

#ifndef INC_SPI_LIB_H_
#define INC_SPI_LIB_H_



#endif /* INC_SPI_LIB_H_ */
#include "main.h"

#define WHO_I_AM 0x0F

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

#define OUT_X_L  0x28
#define OUT_X_H  0x29
#define OUT_Y_L  0x2A
#define OUT_Y_H  0x2B
#define OUT_Z_L  0x2C
#define OUT_Z_H  0x2D


void SPI_Init();
uint8_t SPI_Sensor_Read(uint32_t cmd);
void SPI_Sensor_Write(uint32_t addr, uint8_t data);
void SPI_Multi_Read(char reg, char* buff, int size);
