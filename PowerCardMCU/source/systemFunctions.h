/*
 * systemFunctions.h
 *
 *  Created on: Mar. 16, 2021
 *      Author: Leo Clouet
 */

#ifndef SYSTEMFUNCTIONS_H_
#define SYSTEMFUNCTIONS_H_

unsigned int SystemFunctions_CommitToFlash(unsigned int offset, unsigned int count, unsigned char *data);

unsigned int SystemFunctions_ResetToDefault(unsigned int offset, unsigned int count, unsigned char *data);

unsigned int SystemFunctions_ResetMCU(unsigned int offset, unsigned int count, unsigned char *data);

unsigned int SystemFunctions_GetUptime(unsigned int offset, unsigned int count, unsigned char *data, unsigned int *size);

#endif /* SYSTEMFUNCTIONS_H_ */
