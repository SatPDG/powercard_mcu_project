/*
 * parametersConfig.c
 *
 *  Created on: Feb 1, 2021
 *      Author: RMDS
 */

#include "parametersConfig.h"

#include "bootModule.h"

const paramEntry paramEntryList[] =
{
	{16, bootTimeList, bootTimeDefaultList},
	{16, bootTimeList + 4, bootTimeDefaultList + 4},
	{16, bootTimeList + 8, bootTimeDefaultList + 8},
	{16, bootTimeList + 12, bootTimeDefaultList + 12},
	{16, bootTimeList + 16, bootTimeDefaultList + 16},
	{16, bootTimeList + 20, bootTimeDefaultList + 20},
	{16, bootTimeList + 24, bootTimeDefaultList + 24},
	{16, bootTimeList + 28, bootTimeDefaultList + 28},
};
const unsigned int paramEntryListSize = (sizeof(paramEntryList) / sizeof(paramEntry));
