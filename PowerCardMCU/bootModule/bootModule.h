/*
 * bootModule.h
 *
 *  Created on: Mar. 16, 2021
 *      Author: Leo Clouet
 */

#ifndef BOOTMODULE_H_
#define BOOTMODULE_H_

extern unsigned int bootTimeList[256];
extern const unsigned int bootTimeDefaultList[256];

void BootModule_Init();

void BootModule_Task();

#endif /* BOOTMODULE_H_ */
