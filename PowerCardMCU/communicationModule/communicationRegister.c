/*
 * communicationRegister.c
 *
 *  Created on: Jan. 10, 2021
 *      Author: Leo Clouet
 */

#include "communicationRegister.h"


unsigned int reg1 = 0x11223344;
unsigned int reg2;
unsigned int reg3[256];

comRegister_t registerList[] = {
		{0x1, COM_REGISTER_READ_ACCESS, (unsigned char*) &reg1, 0, 0},
		{0x2, COM_REGISTER_READ_ACCESS | COM_REGISTER_WRITE_ACCESS, (unsigned char*) &reg2, 0, 0},
		{0x3, COM_REGISTER_READ_ACCESS | COM_REGISTER_WRITE_ACCESS | COM_REGISTER_LIST_ACCESS, (unsigned char*) reg3, 0, 0},
};

unsigned int registerListSize = sizeof(registerList) / sizeof(comRegister_t);
