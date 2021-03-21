/*
 * communicationExecutor.c
 *
 *  Created on: Jan. 10, 2021
 *      Author: Leo Clouet
 */

#include "communicationExecutor.h"
#include "communicationRegister.h"
#include "communicationProtocol.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "uart1Driver.h"

unsigned int CommunicationExecutor_ExecuteReadAccess(comRegister_t *regPtr,
		unsigned char offset, unsigned char count, comData_t *responseData);
unsigned int CommunicationExecutor_ExecuteWriteAccess(comRegister_t *regPtr,
		unsigned char offset, unsigned char count, unsigned char *data,
		unsigned int dataSize, comData_t *responseData);

unsigned int CommunicationExecutor_ValidateReadAccess(comRegister_t *regPtr);
unsigned int CommunicationExecutor_ValidateWriteAccess(comRegister_t *regPtr);
unsigned int CommunicationExecutor_ValidateDataAccess(unsigned int dataSize);
unsigned int CommunicationExecutor_ValidateListAccess(comRegister_t *regPtr,
		unsigned int offset, unsigned int count);

unsigned int CommunicationExecutor_Execute(unsigned char function,
		unsigned char registerNo, unsigned char *data, unsigned int dataSize,
		comData_t *responseData)
{
	unsigned int result = 0x1;

	responseData->size = 0;

	// Validate function number
	if (function != cf_read && function != cf_write && function != cf_readList
			&& function != cf_writeList)
	{
		result = 0x0;
		responseData->data[0] = (unsigned char) comerr_badFunction;
		responseData->size = 4;
		return result;
	}

	// Find register
	comRegister_t *regPtr = 0;
	for (unsigned int i = 0; i < registerListSize; i++)
	{
		if (registerList[i].registerNumber == registerNo)
		{
			regPtr = &registerList[i];
			break;
		}
	}

	if (regPtr)
	{
		if (function == cf_read)
		{
			if (dataSize == 0)
			{
				if (CommunicationExecutor_ValidateReadAccess(regPtr))
				{
					result = CommunicationExecutor_ExecuteReadAccess(regPtr, 0,
							1, responseData);
					return result;
				}
				else
				{
					result = 0x0;
					responseData->data[0] =
							(unsigned char) comerr_illegalAccess;
					responseData->size = 4;
					return result;
				}
			}
			else
			{
				result = 0x0;
				responseData->data[0] = (unsigned char) comerr_dataMissing;
				responseData->size = 4;
				return result;
			}
		}
		else if (function == cf_write)
		{
			if (dataSize == 4)
			{
				if (CommunicationExecutor_ValidateWriteAccess(regPtr))
				{
					if(CommunicationExecutor_ValidateDataAccess(dataSize)){
						result = CommunicationExecutor_ExecuteWriteAccess(regPtr, 0,
													1, data, dataSize, responseData);
						return result;
					}
					else
					{
						result = 0x0;
						responseData->data[0] =	(unsigned char) comerr_dataMissing;
						responseData->size = 4;
						return result;
					}
				}
				else
				{
					result = 0x0;
					responseData->data[0] =
							(unsigned char) comerr_illegalAccess;
					responseData->size = 4;
					return result;
				}
			}
			else
			{
				result = 0x0;
				responseData->data[0] = (unsigned char) comerr_dataMissing;
				responseData->size = 4;
				return result;
			}
		}
		else if (function == cf_readList)
		{
			if (dataSize == 2)
			{
				unsigned char offset = data[0];
				unsigned char count = data[1];
				if (CommunicationExecutor_ValidateReadAccess(regPtr)
						&& CommunicationExecutor_ValidateListAccess(regPtr,
								offset, count))
				{
					result = CommunicationExecutor_ExecuteReadAccess(regPtr,
							offset, count, responseData);
					return result;
				}
				else
				{
					result = 0x0;
					responseData->data[0] =
							(unsigned char) comerr_illegalAccess;
					responseData->size = 4;
					return result;
				}
			}
			else
			{
				result = 0x0;
				responseData->data[0] = (unsigned char) comerr_dataMissing;
				responseData->size = 4;
				return result;
			}
		}
		else if (function == cf_writeList)
		{
			if (dataSize >= 2)
			{
				unsigned char offset = data[0];
				unsigned char count = data[1];
				if (CommunicationExecutor_ValidateWriteAccess(regPtr)
						&& CommunicationExecutor_ValidateListAccess(regPtr,
								offset, count))
				{
					if(CommunicationExecutor_ValidateDataAccess(dataSize - 2))
					{
						result = CommunicationExecutor_ExecuteWriteAccess(regPtr,
								offset, count, &data[2], dataSize - 2,
								responseData);
						return result;
					}
					else
					{
						result = 0x0;
						responseData->data[0] =(unsigned char) comerr_dataMissing;
						responseData->size = 1;
						return result;
					}
				}
				else
				{
					result = 0x0;
					responseData->data[0] =
							(unsigned char) comerr_illegalAccess;
					responseData->size = 4;
					return result;
				}
			}
			else
			{
				result = 0x0;
				responseData->data[0] = (unsigned char) comerr_dataMissing;
				responseData->size = 4;
				return result;
			}
		}
	}
	else
	{
		result = 0x0;

		responseData->data[0] = (unsigned char) comerr_regNotFound;
		responseData->size = 4;
		return result;
	}

	return result;
}

unsigned int CommunicationExecutor_ExecuteReadAccess(comRegister_t *regPtr,
		unsigned char offset, unsigned char count, comData_t *responseData)
{
	// Check read function
	if (regPtr->ReadFunction != 0)
	{
		unsigned int size = 0;
		unsigned int buffer[256];
		if (regPtr->ReadFunction(offset, count, (unsigned char*) buffer, &size))
		{
			for (unsigned int i = 0; i < size; i++)
			{
				responseData->data[i] = buffer[i];
			}
			responseData->size = size;
			return 0x1;
		}
		else
		{
			return 0x0;
		}
	}
	else if (regPtr->dataPtr != 0)
	{ // Check ram ptr
		responseData->size = count * sizeof(unsigned int);

		for (unsigned int i = 0; i < responseData->size; i++)
		{
			responseData->data[i] = regPtr->dataPtr[(offset
					* sizeof(unsigned int)) + i];
		}

		return 0x1;
	}
	return 0x0;
}

unsigned int CommunicationExecutor_ExecuteWriteAccess(comRegister_t *regPtr,
		unsigned char offset, unsigned char count, unsigned char *data,
		unsigned int dataSize, comData_t *responseData)
{
	// Check write function
	if (regPtr->WriteFunction != 0)
	{
		if (regPtr->WriteFunction(offset, count, data))
		{
			return 0x1;
		}
	}
	else if (regPtr->dataPtr != 0)
	{
		for (unsigned int i = 0; i < count * sizeof(unsigned int); i++)
		{
			regPtr->dataPtr[(offset * sizeof(unsigned int)) + i] = data[i];
		}
		return 0x1;
	}
	return 0x0;
}

unsigned int CommunicationExecutor_ValidateReadAccess(comRegister_t *regPtr)
{
	return regPtr->permission & COM_REGISTER_READ_ACCESS;
}

unsigned int CommunicationExecutor_ValidateWriteAccess(comRegister_t *regPtr)
{
	return regPtr->permission & COM_REGISTER_WRITE_ACCESS;
}

unsigned int CommunicationExecutor_ValidateDataAccess(unsigned int dataSize)
{
	return (dataSize % 4) == 0;
}

unsigned int CommunicationExecutor_ValidateListAccess(comRegister_t *regPtr,
		unsigned int offset, unsigned int count)
{
	return regPtr->permission & COM_REGISTER_LIST_ACCESS && offset + count < 256;
}
