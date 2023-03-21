/*
 * spi_lib.c
 *
 *  Created on: Jan 5, 2023
 *      Author: Admin
 */

#include "main.h"
#include "spi_lib.h"

uint32_t* GPIOE_ODR = (uint32_t*)0x40021014;
#define SENSOR_INACTIVE  *GPIOE_ODR |= (1 << 3)
#define SENSOR_ACTIVE  *GPIOE_ODR &= ~(1 << 3)




void SPI_Init()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	uint32_t* GPIOA_MODER = (uint32_t*)0x40020000;
	*GPIOA_MODER &= ~(0b111111 << 10);
	*GPIOA_MODER |= (0b10 << 10) | (0b10 << 12) | (0b10 << 14); // set analog
	uint32_t* GPIOA_AFRL = (uint32_t*)0x40020020;
	*GPIOA_AFRL &= ~(0xfff << 20);
	*GPIOA_AFRL |= (5 << 20) | (5 << 24) | (5 << 28);//set function SPI

	__HAL_RCC_GPIOE_CLK_ENABLE();

	uint32_t* GPIOE_MODER = (uint32_t*)0x40021000;
	*GPIOE_MODER &= ~(0b11 << 6);
	*GPIOE_MODER |= (0b01 << 6); //set output get SS SPI;

	SENSOR_INACTIVE;

	__HAL_RCC_SPI1_CLK_ENABLE();

	uint32_t* SPI1_CR1 = (uint32_t*)0x40013000;
	*SPI1_CR1 |= (0b100 << 3); // set CLK 32
	// set bit 2 MSTR chon MASTER
	// set bit 6 SPE:  enable SPI
	// set bit 8 SSI: Internal slave select
	// set bit 9 SSM: Software slave management(Has GPIO)
	*SPI1_CR1 |= (0b1 << 2) | (0b1 << 6) | (0b1 << 8) | (0b1 << 9);

}

uint8_t SPI_Sensor_Read(uint32_t cmd)
{
	uint32_t* SPI1_SR = (uint32_t*)0x40013008;
	uint32_t* SPI1_DR = (uint32_t*)0x4001300c;

	SENSOR_ACTIVE;
	while(((*SPI1_SR >> 1) & 1) != 1); // check data
	uint32_t  data_send = cmd | (0b1 << 7) | (0b1 << 6);
	*SPI1_DR = data_send;

	// return data Recycle Bin
	while(((*SPI1_SR >> 1) & 1) == 1); // check data empty
	while(((*SPI1_SR >> 0) & 1) != 1);
	while(((*SPI1_SR >> 7) & 1) == 1);

	uint8_t temp = *SPI1_DR;

	// return data
	while(((*SPI1_SR >> 1) & 1) != 1);
	*SPI1_DR = 0x00; // set CLK
	while(((*SPI1_SR >> 1) & 1) == 1); // check data empty
	while(((*SPI1_SR >> 0) & 1) != 1);
	while(((*SPI1_SR >> 7) & 1) == 1);

	temp = *SPI1_DR;

	SENSOR_INACTIVE;
	return temp;
}

void SPI_Sensor_Write(uint32_t addr, uint8_t data)
{
	uint32_t* SPI1_SR = (uint32_t*)0x40013008;
	uint32_t* SPI1_DR = (uint32_t*)0x4001300c;

	SENSOR_ACTIVE;
	while(((*SPI1_SR >> 1) & 1) != 1); // check data
	*SPI1_DR = addr;
	while(((*SPI1_SR >> 1) & 1) == 1); // check data empty
	while(((*SPI1_SR >> 0) & 1) != 1);
	while(((*SPI1_SR >> 7) & 1) == 1);

	while(((*SPI1_SR >> 1) & 1) != 1); // check data
	*SPI1_DR = data;
	while(((*SPI1_SR >> 1) & 1) == 1); // check data empty
	while(((*SPI1_SR >> 0) & 1) != 1);
	while(((*SPI1_SR >> 7) & 1) == 1);

	SENSOR_INACTIVE;
}

void SPI_Multi_Read(char reg, char* buff, int size)
{
	SENSOR_ACTIVE;
	uint32_t* retVen = SPI_Sensor_Read(reg);
	for(int i = 0; i < size; i++)
	{
		buff[i] = retVen[i];
	}
	SENSOR_INACTIVE;
}
