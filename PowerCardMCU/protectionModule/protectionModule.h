/*
 * protectionModule.h
 *
 *  Created on: Feb. 16, 2021
 *      Author: Leo Clouet
 */

#ifndef PROTECTIONMODULE_H_
#define PROTECTIONMODULE_H_

#define PROTECTION_OK 0x1
#define PROTECTION_NO_CURRENT 0x0

extern unsigned int protectionStateList[256];

void ProtectionModule_Init();

void ProtectionModule_Task();

#endif /* PROTECTIONMODULE_H_ */
