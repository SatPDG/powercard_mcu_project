/*
 * parametersConfig.h
 *
 *  Created on: Feb 1, 2021
 *      Author: Leo Clouet
 */

#ifndef PARAMETERSCONFIG_H_
#define PARAMETERSCONFIG_H_

typedef struct
{
	unsigned int size;
	unsigned char *ramPtr;
	unsigned char *defaultPtr;
} paramEntry;

extern const paramEntry paramEntryList[];
extern const unsigned int paramEntryListSize;

#endif /* PARAMETERSCONFIG_H_ */
