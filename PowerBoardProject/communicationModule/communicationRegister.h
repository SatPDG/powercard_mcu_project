/*
 * communicationRegister.h
 *
 *  Created on: Jan. 10, 2021
 *      Author: Leo Clouet
 */

#ifndef COMMUNICATION_REGISTER_H
#define COMMUNICATION_REGISTER_H

#define COM_REGISTER_READ_ACCESS 0x1
#define COM_REGISTER_WRITE_ACCESS 0x2
#define COM_REGISTER_LIST_ACCESS 0x4

typedef struct {
	unsigned char registerNumber;
	unsigned short permission;
	unsigned char *dataPtr;
	unsigned int (*ReadFunction)(unsigned int offset, unsigned int count, unsigned char *data, unsigned int *size);
	unsigned int (*WriteFunction)(unsigned int offset, unsigned int count, unsigned char *data);
} comRegister_t;

extern comRegister_t registerList[];
extern unsigned int registerListSize;

#endif
