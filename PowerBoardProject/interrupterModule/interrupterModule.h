/*
 * interrupterModule.h
 *
 *  Created on: Feb. 16, 2021
 *      Author: Leo Clouet
 */

#ifndef INTERRUPTERMODULE_H_
#define INTERRUPTERMODULE_H_

extern unsigned char interrupterStateList[256];

void InterrupterModule_Init();

void InterrupterModule_UpdateState();

#endif /* INTERRUPTERMODULE_H_ */
