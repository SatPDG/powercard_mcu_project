/*
 * protectionModule.h
 *
 *  Created on: Feb. 16, 2021
 *      Author: Leo Clouet
 */

#ifndef PROTECTIONMODULE_H_
#define PROTECTIONMODULE_H_

extern unsigned int protectionStateList[256];
extern unsigned int protectionOverCurrentList[256];

void ProtectionModule_Init();

void ProtectionModule_Task();

#endif /* PROTECTIONMODULE_H_ */
