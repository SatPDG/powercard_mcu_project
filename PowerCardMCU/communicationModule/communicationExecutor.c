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
unsigned int CommunicationExecutor_ValidateDataAccess(unsigned int dataSize,
		unsigned int count);
unsigned int CommunicationExecutor_ValidateListAccess(comRegister_t *regPtr,
		unsigned int offset, unsigned int count);

unsigned int CommunicationExecutor_Execute(unsigned char function,
		unsigned char registerNo, unsigned char *data, unsigned int dataSize,
		comData_t *responseData)
{
	unsigned int result = 0x1;

	responseData->size = 0;

	// Validate the function number.
	if (function != cf_read && function != cf_write && function != cf_readList
			&& function != cf_writeList)
	{
		result = 0x0;
		responseData->data[0] = (unsigned char) comerr_badFunction;
		responseData->size = 4;
		return result;
	}

	// Find the register.
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
		// If a register was found.
		if (function == cf_read)
		{
			// Check that no data was send for this command.
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
					// The read access is not permitted for this register.
					result = 0x0;
					responseData->data[0] =
							(unsigned char) comerr_illegalAccess;
					responseData->size = 4;
					return result;
				}
			}
			else
			{
				// Some data was send with the message.
				result = 0x0;
				responseData->data[0] = (unsigned char) comerr_dataMissing;
				responseData->size = 4;
				return result;
			}
		}
		else if (function == cf_write)
		{
			// Check that the data to write only one register was sent.
			if (dataSize == 4)
			{
				if (CommunicationExecutor_ValidateWriteAccess(regPtr))
				{
					if (CommunicationExecutor_ValidateDataAccess(dataSize, 1))
					{
						result = CommunicationExecutor_ExecuteWriteAccess(
								regPtr, 0, 1, data, dataSize, responseData);
						return result;
					}
					else
					{
						// Some extra or not enough data was sent to write a single register.
						result = 0x0;
						responseData->data[0] =
								(unsigned char) comerr_dataMissing;
						responseData->size = 4;
						return result;
					}
				}
				else
				{
					// The register write access is not permit.
					result = 0x0;
					responseData->data[0] =
							(unsigned char) comerr_illegalAccess;
					responseData->size = 4;
					return result;
				}
			}
			else
			{
				// Some extra or not enough data was sent to write a single register.
				result = 0x0;
				responseData->data[0] = (unsigned char) comerr_dataMissing;
				responseData->size = 4;
				return result;
			}
		}
		else if (function == cf_readList)
		{
			// Make sure only 2 data was sent. The offset and count.
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
					// The register does not permit a list or read access.
					result = 0x0;
					responseData->data[0] =
							(unsigned char) comerr_illegalAccess;
					responseData->size = 4;
					return result;
				}
			}
			else
			{
				// Not enough or too much data was sent.
				result = 0x0;
				responseData->data[0] = (unsigned char) comerr_dataMissing;
				responseData->size = 4;
				return result;
			}
		}
		else if (function == cf_writeList)
		{
			// Make sure that at least 2 data was sent. The offset and the count.
			if (dataSize >= 2)
			{
				unsigned char offset = data[0];
				unsigned char count = data[1];
				if (CommunicationExecutor_ValidateWriteAccess(regPtr)
						&& CommunicationExecutor_ValidateListAccess(regPtr,
								offset, count))
				{
					if (CommunicationExecutor_ValidateDataAccess(dataSize - 2,
							count))
					{
						result = CommunicationExecutor_ExecuteWriteAccess(
								regPtr, offset, count, &data[2], dataSize - 2,
								responseData);
						return result;
					}
					else
					{
						// Not enough or too much data was sent.
						result = 0x0;
						responseData->data[0] =
								(unsigned char) comerr_dataMissing;
						responseData->size = 4;
						return result;
					}
				}
				else
				{
					// The register does not permit a list or write access.
					result = 0x0;
					responseData->data[0] =
							(unsigned char) comerr_illegalAccess;
					responseData->size = 4;
					return result;
				}
			}
			else
			{
				// Not enough or too much data was sent.
				result = 0x0;
				responseData->data[0] = (unsigned char) comerr_dataMissing;
				responseData->size = 4;
				return result;
			}
		}
	}
	else
	{
		// No regiter was found.
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
	// Check read function.
	if (regPtr->ReadFunction != 0)
	{
		unsigned int size = 0;
		unsigned int buffer[256];
		if (regPtr->ReadFunction(offset, count, (unsigned char*) buffer, &size))
		{
			// Copy paste the function data return in the response buffer.
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
	{
		// Check ram ptr
		responseData->size = count * sizeof(unsigned int);

		// Copy paste the register data in the response data.
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
	// Check write function.
	if (regPtr->WriteFunction != 0)
	{
		if (regPtr->WriteFunction(offset, count, data))
		{
			return 0x1;
		}
	}
	else if (regPtr->dataPtr != 0)
	{
		// Check the ram ptr.
		// Copy paste the sent data in the register.
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

unsigned int CommunicationExecutor_ValidateDataAccess(unsigned int dataSize,
		unsigned int count)
{
	// Make sure that the sent data is a equal to the number of register target by the command.
	return dataSize == 4 * count;
}

unsigned int CommunicationExecutor_ValidateListAccess(comRegister_t *regPtr,
		unsigned int offset, unsigned int count)
{
	// Make sure the list access is permitted and that the offset and count does not exceed the list upper limit.
	return regPtr->permission & COM_REGISTER_LIST_ACCESS && offset + count < 256;
}
