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
	unsigned char registerNumber; 	// The register number.
	unsigned short permission;		// The register permission. Defined by the macro that end with _ACCESS.
	unsigned char *dataPtr;			// The pointer that point to the register data.
	// The read function. If  function is defined, the function is called instead of reading the data from the register pointer.
	unsigned int (*ReadFunction)(unsigned int offset, unsigned int count, unsigned char *data, unsigned int *size);
	// Teh write function. if a function is defined, the function is called instead of writing the data directly to the register pointer.
	unsigned int (*WriteFunction)(unsigned int offset, unsigned int count, unsigned char *data);
} comRegister_t;

extern comRegister_t registerList[];
extern unsigned int registerListSize;

#endif
