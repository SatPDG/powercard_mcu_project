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
#include "bootModule.h"
#include "systemFunctions.h"
#include "ledModule.h"

typedef enum
{
	register_tension = 0x1,
	register_current = 0x2,
	register_power = 0x3,
	register_temperature = 0x10,
	register_protectionState = 0x30,
	register_interrupter = 0x50,
	register_autoResetInterupter = 0x51,
	register_bootTime = 0x60,
	register_commitToFlash = 0x70,
	register_resetToDefault = 0x71,
	register_resetMCU = 0x72,
	register_uptime = 0x74,

} com_register;

comRegister_t registerList[] = {
		{register_tension, COM_REGISTER_READ_ACCESS, (unsigned char*) &voltage, 0, 0},
		{register_current, COM_REGISTER_READ_ACCESS, (unsigned char *) &current, 0, 0},
		{register_power, COM_REGISTER_READ_ACCESS, (unsigned char *) &power, 0, 0},
		{register_temperature, COM_REGISTER_READ_ACCESS, (unsigned char *) &temperature, 0, 0},
		{register_protectionState, COM_REGISTER_READ_ACCESS | COM_REGISTER_LIST_ACCESS, (unsigned char *) protectionStateList, 0, 0},
		{register_interrupter, COM_REGISTER_READ_ACCESS | COM_REGISTER_WRITE_ACCESS | COM_REGISTER_LIST_ACCESS, (unsigned char *) interrupterStateList, 0, InterrupterModule_SetInterrupters},
		{register_autoResetInterupter, COM_REGISTER_READ_ACCESS | COM_REGISTER_WRITE_ACCESS | COM_REGISTER_LIST_ACCESS, (unsigned char *) autoResetInterruptList, 0, 0},
		{register_bootTime, COM_REGISTER_READ_ACCESS | COM_REGISTER_WRITE_ACCESS | COM_REGISTER_LIST_ACCESS, (unsigned char *) bootTimeList, 0, 0},
		{register_commitToFlash, COM_REGISTER_WRITE_ACCESS, 0, 0, SystemFunctions_CommitToFlash},
		{register_resetToDefault, COM_REGISTER_WRITE_ACCESS, 0, 0, SystemFunctions_ResetToDefault},
		{register_resetMCU, COM_REGISTER_WRITE_ACCESS, 0, 0, SystemFunctions_ResetMCU},
		{register_uptime, COM_REGISTER_READ_ACCESS, (unsigned char *) &uptime, 0, 0},
};

unsigned int registerListSize = sizeof(registerList) / sizeof(comRegister_t);
