/*
 * communicationRegister.c
 *
 *  Created on: Jan. 10, 2021
 *      Author: Leo Clouet
 */

#include "communicationRegister.h"

#include "samplingModule.h"
#include "protectionModule.h"
#include "interrupterModule.h"

unsigned int reg1 = 0x11223344;
unsigned int reg2;
unsigned int reg3[256];

comRegister_t registerList[] = {
		{0x1, COM_REGISTER_READ_ACCESS, (unsigned char*) &voltage, 0, 0},
		{0x2, COM_REGISTER_READ_ACCESS, (unsigned char *) &current, 0, 0},
		{0x3, COM_REGISTER_READ_ACCESS, (unsigned char *) &power, 0, 0},
		{0x10, COM_REGISTER_READ_ACCESS, (unsigned char *) &temperature, 0, 0},
		{0x30, COM_REGISTER_READ_ACCESS | COM_REGISTER_LIST_ACCESS, (unsigned char *) protectionStateList, 0, 0},
		{0x31, COM_REGISTER_READ_ACCESS | COM_REGISTER_LIST_ACCESS, (unsigned char *) protectionOverCurrentList, 0, 0},
		{0x50, COM_REGISTER_READ_ACCESS | COM_REGISTER_WRITE_ACCESS | COM_REGISTER_LIST_ACCESS, (unsigned char *) interrupterStateList, 0, 0},
};

unsigned int registerListSize = sizeof(registerList) / sizeof(comRegister_t);
