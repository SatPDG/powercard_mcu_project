/*
 * systemFunctions.c
 *
 *  Created on: Mar. 16, 2021
 *      Author: Leo Clouet
 */

#define SYSTEM_PASSWORD 0x74AB

#include "systemFunctions.h"

#include "parameterSystem.h"

#include "MIMXRT1021.h"
#include "FreeRTOS.h"
#include "task.h"

unsigned int SystemFunctions_CommitToFlash(unsigned int offset, unsigned int count, unsigned char *data)
{
	// Validate the password.
	unsigned int passwordSent = *((unsigned int*) data);
	if(count == 1 && passwordSent == SYSTEM_PASSWORD)
	{
		ParameterSystem_CommitToFlash();
	}
	else
	{
		return 0;
	}
	return 1;
}

unsigned int SystemFunctions_ResetToDefault(unsigned int offset, unsigned int count, unsigned char *data)
{
	// Validate the password.
	unsigned int passwordSent = *((unsigned int*) data);
	if(count == 1 && passwordSent == SYSTEM_PASSWORD)
	{
		ParameterSystem_ResetToDefault();
	}
	else
	{
		return 0;
	}
	return 1;
}

unsigned int SystemFunctions_ResetMCU(unsigned int offset, unsigned int count, unsigned char *data)
{
	// Validate the password.
	unsigned int passwordSent = *((unsigned int*) data);
	if(count == 1 && passwordSent == SYSTEM_PASSWORD)
	{
		__disable_irq();
		__NVIC_SystemReset();
		while(1);
	}
	else
	{
		return 0;
	}
	return 1;
}

unsigned int SystemFunctions_GetUptime(unsigned int offset, unsigned int count, unsigned char *data, unsigned int *size)
{
//	data[0] = 1;
//	data[0] = 2;
//	data[0] = 3;
//	data[0] = 4;
	//unsigned int *uptime = (unsigned int *) data;
	//uptime[0] = xTaskGetTickCount();
	//*size = 4;
	return 1;
}
