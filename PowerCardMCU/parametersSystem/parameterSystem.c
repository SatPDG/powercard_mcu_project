/*
 * parameterSystem.c
 *
 *  Created on: Feb 1, 2021
 *      Author: RMDS
 */

#include "parameterSystem.h"

#include "parametersConfig.h"

#include "crc16.h"

#include "flashDriver.h"
#include "flashMemoryMap.h"

#define PARAMETER_SYSTEM_RESERVED_1_VAL 0x1122
#define PARAMETER_SYSTEM_RESERVED_2_VAL 0xAABBCCDD
#define PARAMETER_SYSTEM_RESERVED_3_VAL 0x99887766

typedef struct
{
	unsigned short crc;
	unsigned short reserved1;
	unsigned int entryCount;
	unsigned int reserved2;
	unsigned int reserved3;
} flashHeader;


typedef struct
{
	unsigned char val[16];
} flashParamEntry;

typedef struct
{
	flashHeader header;
	flashParamEntry entry[255];
} flashParamSystem;

unsigned int ParameterSystem_IsValidSystem(unsigned char *buffer);
unsigned short ParameterSystem_GetCrc(unsigned char *buffer);

unsigned char flashBuffer[4096];

void ParameterSystem_Init()
{
	ParameterSystem_Load();
}

void ParameterSystem_Load()
{
	 // Check system A validity
	FlashDriver_ReadBytes(FLASH_PARAMETERS_A_ADDRESS, flashBuffer, 4096);
	unsigned int paramAIsValid = ParameterSystem_IsValidSystem(flashBuffer);

	// Check system B validity
	FlashDriver_ReadBytes(FLASH_PARAMETERS_B_ADDRESS, flashBuffer, 4096);
	unsigned int paramBIsValid = ParameterSystem_IsValidSystem(flashBuffer);

	if(!paramAIsValid && !paramBIsValid)
	{
		// Both of system param are invalid.

		// Do nothing for the flash, just load default values.
		ParameterSystem_ResetToDefault();
		return;
	}
	else if(!paramAIsValid && paramBIsValid)
	{
		// Param A are invalid.

		// Overwrite param A by param B
		FlashDriver_ReadBytes(FLASH_PARAMETERS_B_ADDRESS, flashBuffer, 4096);
		FlashDriver_EraseSector(FLASH_PARAMETERS_A_ADDRESS);
		for(unsigned int i = 0; i < (FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE); i++)
		{
			FlashDriver_ProgramPage(FLASH_PARAMETERS_A_ADDRESS + (i * FLASH_PAGE_SIZE), flashBuffer + (i * FLASH_PAGE_SIZE));
		}
	}
	else if(paramAIsValid && !paramBIsValid)
	{
		// Param B are invalid.

		// Overwrite param A by param B
		FlashDriver_ReadBytes(FLASH_PARAMETERS_A_ADDRESS, flashBuffer, 4096);
		FlashDriver_EraseSector(FLASH_PARAMETERS_B_ADDRESS);
		for(unsigned int i = 0; i < (FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE); i++)
		{
			FlashDriver_ProgramPage(FLASH_PARAMETERS_B_ADDRESS + (i * FLASH_PAGE_SIZE), flashBuffer + (i * FLASH_PAGE_SIZE));
		}
	}
	else
	{
		// Both param are valid.

		// Do nothing.
	}

	// Load param in flash
	FlashDriver_ReadBytes(FLASH_PARAMETERS_A_ADDRESS, flashBuffer, 4096);
	flashHeader *header = (flashHeader*) flashBuffer;
	flashParamEntry *entryList = (flashParamEntry*) flashBuffer + sizeof(flashHeader);

	for(unsigned int i = 0; i < paramEntryListSize && i < header->entryCount; i++)
	{
		flashParamEntry *flashEntry = &entryList[i];
		const paramEntry *param = &paramEntryList[i];

		if(param->ramPtr != 0){
			// Copy param value in param
			memcpy(param->ramPtr, flashEntry->val, param->size);
		}
	}
}

void ParameterSystem_ResetToDefault()
{
	FlashDriver_EraseSector(FLASH_PARAMETERS_A_ADDRESS);
	FlashDriver_EraseSector(FLASH_PARAMETERS_B_ADDRESS);

	for(unsigned int i = 0; i < paramEntryListSize; i++)
	{
		const paramEntry *param = &paramEntryList[i];
		if(param->ramPtr != 0 && param->defaultPtr != 0)
		{
			memcpy(param->ramPtr, param->defaultPtr, param->size);
		}
	}
}

void ParameterSystem_CommitToFlash()
{
	FlashDriver_EraseSector(FLASH_PARAMETERS_A_ADDRESS);
	FlashDriver_EraseSector(FLASH_PARAMETERS_B_ADDRESS);

	FlashDriver_ReadBytes(FLASH_PARAMETERS_A_ADDRESS, flashBuffer, 4096);

	flashHeader *header = (flashHeader*) flashBuffer;
	flashParamEntry *entryList = (flashParamEntry*) flashBuffer + sizeof(flashHeader);

	// Copy the param val in the flash buffer.
	for(unsigned int i = 0; i < paramEntryListSize; i++)
	{
		if(paramEntryList[i].ramPtr != 0)
		{
			memcpy(entryList[i].val, paramEntryList[i].ramPtr, paramEntryList[i].size);
		}
	}

	// Initialize the header
	header->entryCount = paramEntryListSize;
	header->reserved1 = PARAMETER_SYSTEM_RESERVED_1_VAL;
	header->reserved2 = PARAMETER_SYSTEM_RESERVED_2_VAL;
	header->reserved3 = PARAMETER_SYSTEM_RESERVED_3_VAL;

	// Compute the crc.
	header->crc = ParameterSystem_GetCrc(flashBuffer);

	// Commit the buffer to flash
	for(unsigned int i = 0; i < (FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE); i++)
	{
		FlashDriver_ProgramPage(FLASH_PARAMETERS_A_ADDRESS + (i * FLASH_PAGE_SIZE), flashBuffer + (i * FLASH_PAGE_SIZE));
		FlashDriver_ProgramPage(FLASH_PARAMETERS_B_ADDRESS + (i * FLASH_PAGE_SIZE), flashBuffer + (i * FLASH_PAGE_SIZE));
	}
}

unsigned int ParameterSystem_IsValidSystem(unsigned char *buffer)
{
	unsigned int isValid = 0x0;
	// Check if the reserved field have the right values and if the size if possible.
	flashHeader *header = (flashHeader*) buffer;
	if(header->reserved1 == PARAMETER_SYSTEM_RESERVED_1_VAL && header->reserved2 == PARAMETER_SYSTEM_RESERVED_2_VAL && header->reserved3 == PARAMETER_SYSTEM_RESERVED_3_VAL)
	{
		if(header->entryCount < 256)
		{
			// Check crc
			unsigned short crc = ParameterSystem_GetCrc(buffer);
			if(crc == header->crc)
			{
				isValid = 0x1;
			}
		}
	}
	return isValid;
}

unsigned short ParameterSystem_GetCrc(unsigned char *buffer)
{
	unsigned short crc = 0x0000;
	crc = CRC16_Compute(buffer + 2, FLASH_SECTOR_SIZE - 2);
	return crc;
}
