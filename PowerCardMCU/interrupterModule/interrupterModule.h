/*
 * interrupterModule.h
 *
 *  Created on: Feb. 16, 2021
 *      Author: Leo Clouet
 */

#ifndef INTERRUPTERMODULE_H_
#define INTERRUPTERMODULE_H_

#define INTERRUPTER_CLOSE 0x00
#define INTERRUPTER_OPEN 0x01
#define INTERRUPTER_RESET 0x02

extern unsigned int interrupterStateList[256];
extern unsigned int autoResetInterruptList[256];
extern const unsigned int autoResetDefaultList[256];

void InterrupterModule_Init();

unsigned int InterrupterModule_SetInterrupters(unsigned int offset, unsigned int count, unsigned char *data);

void InterrupterModule_CloseAllInterrupter();

void InterrupterModule_UpdateInterrupterState(unsigned int interrupter, unsigned int state);

void InterrupterModule_AutoResetTask();

#endif /* INTERRUPTERMODULE_H_ */
