/*
 * parametersConfig.c
 *
 *  Created on: Feb 1, 2021
 *      Author: RMDS
 */

#include "parametersConfig.h"

#include "bootModule.h"
#include "interrupterModule.h"

const paramEntry paramEntryList[] =
{
	{16, (unsigned char*) bootTimeList, (unsigned char*) bootTimeDefaultList},
	{16, (unsigned char*) (bootTimeList + 4), (unsigned char*) (bootTimeDefaultList + 4)},
	{16, (unsigned char*) (bootTimeList + 8), (unsigned char*) (bootTimeDefaultList + 8)},
	{16, (unsigned char*) (bootTimeList + 12), (unsigned char*) (bootTimeDefaultList + 12)},
	{16, (unsigned char*) (bootTimeList + 16), (unsigned char*) (bootTimeDefaultList + 16)},
	{16, (unsigned char*) (bootTimeList + 20), (unsigned char*) (bootTimeDefaultList + 20)},
	{16, (unsigned char*) (bootTimeList + 24), (unsigned char*) (bootTimeDefaultList + 24)},
	{16, (unsigned char*) (bootTimeList + 28), (unsigned char*) (bootTimeDefaultList + 28)},
	{16, (unsigned char*) autoResetInterruptList, (unsigned char*) autoResetDefaultList},
	{16, (unsigned char*) (autoResetInterruptList + 4), (unsigned char*) (autoResetDefaultList + 4)},
	{16, (unsigned char*) (autoResetInterruptList + 8), (unsigned char*) (autoResetDefaultList + 8)},
	{16, (unsigned char*) (autoResetInterruptList + 12), (unsigned char*) (autoResetDefaultList + 12)},
	{16, (unsigned char*) (autoResetInterruptList + 16), (unsigned char*) (autoResetDefaultList + 16)},
	{16, (unsigned char*) (autoResetInterruptList + 20), (unsigned char*) (autoResetDefaultList + 20)},
	{16, (unsigned char*) (autoResetInterruptList + 24), (unsigned char*) (autoResetDefaultList + 24)},
	{16, (unsigned char*) (autoResetInterruptList + 28), (unsigned char*) (autoResetDefaultList + 28)},
};
const unsigned int paramEntryListSize = (sizeof(paramEntryList) / sizeof(paramEntry));
