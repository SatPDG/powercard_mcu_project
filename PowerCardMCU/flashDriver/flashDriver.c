/*
 * flashDriver.c
 *
 *  Created on: Feb 1, 2021
 *      Author: Leo Clouet
 *
 *  This file allow the program to program, read and erase some sector of the flash
 *  while the MCU is executing the code from the FLASH.
 *
 *  To achieve this, some special instruction must be always done before any FLASH special action.
 *  Those special instruction are in the PreventExecutionOfCodeFromFlash function.
 *
 *  This file and some other file are also executed in from the ram and not the FLASH.
 *  The files in the linkscripts directory manage that.
 */

#include "flashDriver.h"

#include "flashMemoryMap.h"
#include "flashDriverOps.h"
#include "fsl_iomuxc.h"
#include "fsl_cache.h"
#include "fsl_clock.h"

#include <cr_section_macros.h>

void PreventExecutionOfCodeFromFlash();
void RestoreExecutionOfCodeFromFlash();

status_t FlashDriver_Init()
{
	// Init IOMUXC for FLASH
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_05_FLEXSPI_A_DQS, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_06_FLEXSPI_A_DATA03, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_07_FLEXSPI_A_SCLK, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_08_FLEXSPI_A_DATA00, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_09_FLEXSPI_A_DATA02, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_FLEXSPI_A_DATA01, 1U);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_FLEXSPI_A_SS0_B, 1U);

	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_05_FLEXSPI_A_DQS, 0x10F1u);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_06_FLEXSPI_A_DATA03, 0x10F1u);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_07_FLEXSPI_A_SCLK, 0x10F1u);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_08_FLEXSPI_A_DATA00, 0x10F1u);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_09_FLEXSPI_A_DATA02, 0x10F1u);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_10_FLEXSPI_A_DATA01, 0x10F1u);
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_11_FLEXSPI_A_SS0_B, 0x10F1u);

	PreventExecutionOfCodeFromFlash();

	flexspi_nor_flash_init(FLEXSPI);
	status_t status = flexspi_nor_enable_quad_mode(FLEXSPI);

	RestoreExecutionOfCodeFromFlash();

	return status;
}

void FlashDriver_ReadBytesBySector(unsigned int sector, unsigned int page, unsigned char *buffer, unsigned int size)
{
	memcpy(buffer, (void *) (FLASH_BASE_ADDRESS + (sector * FLASH_SECTOR_SIZE) + (page * FLASH_PAGE_SIZE)), size);
}

void FlashDriver_ReadBytes(unsigned int address, unsigned char *buffer, unsigned int size)
{
	unsigned char *dataPtr = (unsigned char *) address;
	for(unsigned int i = 0; i < size; i++)
	{
		buffer[i] = *dataPtr;
		dataPtr++;
	}
}

status_t FlashDriver_ProgramPageBySector(unsigned int sector, unsigned int page, unsigned char *buffer)
{
	PreventExecutionOfCodeFromFlash();
	status_t status = flexspi_nor_flash_page_program(FLEXSPI, (sector * FLASH_SECTOR_SIZE) + (page * FLASH_PAGE_SIZE), (void*) buffer);
	RestoreExecutionOfCodeFromFlash();
	return status;
}

status_t FlashDriver_ProgramPage(unsigned int address, unsigned char *buffer)
{
	PreventExecutionOfCodeFromFlash();
	status_t status = flexspi_nor_flash_page_program(FLEXSPI, address - FLASH_BASE_ADDRESS, (void*) buffer);
	RestoreExecutionOfCodeFromFlash();
	return status;
}

status_t FlashDriver_EraseSectorBySector(unsigned int sector)
{
	PreventExecutionOfCodeFromFlash();
	status_t status = flexspi_nor_flash_erase_sector(FLEXSPI, sector * FLASH_SECTOR_SIZE);
	RestoreExecutionOfCodeFromFlash();
	return status;
}

status_t FlashDriver_EraseSector(unsigned int address)
{
	PreventExecutionOfCodeFromFlash();
	status_t status = flexspi_nor_flash_erase_sector(FLEXSPI, address - FLASH_BASE_ADDRESS);
	RestoreExecutionOfCodeFromFlash();
	return status;
}

void PreventExecutionOfCodeFromFlash()
{
	// This function is a barrier that prevent any FLASH code execution.
	// Since the code is in the FLASH, the MCU cannot program/erase the FLASH and read code instruction at the same time.
	__disable_irq();
	SCB_DisableDCache();	// Disable all cache so no FLASH write is done.
	SCB_DisableICache();
	// The two following instruction make sure all the FLASH access are finish.
	__DSB(); // Force completion of all data access
	__ISB(); // Flush pipeline
}

void RestoreExecutionOfCodeFromFlash()
{
	SCB_InvalidateICache();		// Enable the cache.
	SCB_EnableICache();
	SCB_InvalidateDCache();
	SCB_EnableDCache();
	__DSB(); // Force completion of all data access
	__ISB(); // Flush pipeline
  __enable_irq();
}
