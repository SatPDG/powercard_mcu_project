/*
 * communicationParser.h
 *
 *  Created on: Jan. 17, 2021
 *      Author: Leo Clouet
 */

#ifndef COMMUNICATIONPARSER_H_
#define COMMUNICATIONPARSER_H_

#include "communicationProtocol.h"

typedef enum
{
	parser_startByte,
	parser_size,
	parser_function,
	parser_register,
	parser_data,
	parser_crc1,
	parser_crc2,
} comParserState;

typedef struct
{
	comParserState state;
	comPacket_t packet;
	unsigned int receivedData;
	unsigned short crc;
} comParser_t;

extern comParser_t parser;

void CommunicationParser_Init();

unsigned int CommunicationParser_Parse(unsigned char *data, unsigned int size);

void CommunicationParser_Flush();

#endif /* COMMUNICATIONPARSER_H_ */
