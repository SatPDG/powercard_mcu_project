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
}
