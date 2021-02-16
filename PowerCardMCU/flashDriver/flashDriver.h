/*
 * flashDriver.h
 *
 *  Created on: Feb 1, 2021
 *      Author: RMDS
 */

#ifndef FLASHDRIVER_H_
#define FLASHDRIVER_H_

#include "fsl_common.h"

status_t FlashDriver_Init();

void FlashDriver_ReadBytesBySector(unsigned int sector, unsigned int page, unsigned char *buffer, unsigned int size);

void FlashDriver_ReadBytes(unsigned int address, unsigned char *buffer, unsigned int size);

// Buffer must be 256 bytes in length.
status_t FlashDriver_ProgramPageBySector(unsigned int sector, unsigned int page, unsigned char *buffer);

// Buffer must be 256 bytes in length.
status_t FlashDriver_ProgramPage(unsigned int address, unsigned char *buffer);

status_t FlashDriver_EraseSectorBySector(unsigned int sector);

status_t FlashDriver_EraseSector(unsigned int address);

#endif /* FLASHDRIVER_H_ */
