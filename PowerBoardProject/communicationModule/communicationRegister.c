/*
 * communicationRegister.c
 *
 *  Created on: Jan. 10, 2021
 *      Author: Leo Clouet
 */

#include "communicationRegister.h"

comRegister_t registerList[] = {
		{0x0, COM_REGISTER_READ_ACCESS | COM_REGISTER_WRITE_ACCESS, 0, 0, 0},
};
unsigned int registerListSize = sizeof(registerList) / sizeof(comRegister_t);
