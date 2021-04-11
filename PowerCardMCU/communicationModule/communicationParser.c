/*
 * communicationParser.c
 *
 *  Created on: Jan. 17, 2021
 *      Author: Leo Clouet
 */

#include "communicationParser.h"
#include "crc16.h"

void CommunicationParser_Init(comParser_t *parser)
{
	parser->state = parser_startByte;
	parser->packet.data = 0;
	parser->packet.size = 0;
	parser->receivedData = 0;
}

unsigned int CommunicationParser_Parse(comParser_t *parser, unsigned char *data, unsigned int size)
{
	unsigned int result = 0;
	for (unsigned int i = 0; i < size; i++)
	{
		unsigned char byte = data[i];

		switch (parser->state)
		{
		case parser_startByte:
		{
			// If the data was not free yet, free it.
			if(parser->packet.data != 0){
				vPortFree(parser->packet.data);
				parser->packet.data = 0;
			}

			parser->crc = 0;
			parser->receivedData = 0;

			if (byte == COM_START_BYTE)
			{
				parser->state = parser_size;
			}
			break;
		}
		case parser_size:
		{
			parser->packet.size = byte;
			parser->state = parser_function;
			break;
		}
		case parser_function:
		{
			parser->packet.numSeq = (byte >> 4);
			parser->packet.numFunction = (byte & 0xF);
			parser->state = parser_register;
			break;
		}
		case parser_register:
		{
			parser->packet.numRegister = byte;
			if (parser->packet.size == 4)
			{
				parser->state = parser_crc1;

			}
			else
			{
				parser->state = parser_data;
			}
			break;
		}
		case parser_data:
		{
			// If no data was allocated for the data field, allocate it.
			if (parser->packet.data == 0)
			{
				parser->packet.data = pvPortMalloc(parser->packet.size - 4);
			}
			parser->packet.data[parser->receivedData] = byte;
			parser->receivedData++;

			if (parser->receivedData == parser->packet.size - 4)
			{
				parser->state = parser_crc1;
			}
			break;
		}
		case parser_crc1:
		{
			parser->crc = ((unsigned short) byte) << 8;
			parser->state = parser_crc2;
			break;
		}
		case parser_crc2:
		{
			parser->crc |= byte;

			unsigned char startBuf[4] =
			{ COM_START_BYTE, parser->packet.size, parser->packet.numSeq << 4
					| parser->packet.numFunction, parser->packet.numRegister };
			unsigned short computedCrc = CRC16_Compute(startBuf, 4);
			computedCrc = CRC16_Accumulate(computedCrc, parser->packet.data, parser->receivedData);

			parser->state = parser_startByte;

			if(parser->crc == computedCrc){
				// If a valid message was found, return right away to
				// evitate the free on the data in the start byte case.
				result = 0x1;
				return result;
			}
			else
			{
				result = 0x0;
			}
			break;
		}
		}
	}
	return result;
}

void CommunicationParser_Flush(comParser_t *parser)
{
	// Just reset the paser state, the data will be freed in the start byte case.
	parser->state = parser_startByte;
}
