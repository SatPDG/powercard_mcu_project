/*
 * bootModule.c
 *
 *  Created on: Mar. 16, 2021
 *      Author: Leo Clouet
 */

#include "bootModule.h"

#include "FreeRTOS.h"
#include "task.h"
#include "interrupterModule.h"

unsigned int bootTimeList[256];
const unsigned int bootTimeDefaultList[256] =
{
	100
};

unsigned int bootTickList[256];
unsigned char bootHandle[256];

void BootModule_Init()
{

}

void BootModule_Task()
{
	// Convert ms to system tick.
	for(unsigned int i = 0; i < 256; i++)
	{
		bootHandle[i] = 0;
		bootTickList[i] = pdMS_TO_TICKS(bootTimeList[i]);
	}


	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(200));
		unsigned int currentTick = xTaskGetTickCount();
		unsigned char hasUpdateInterrupter = 0;

		for(unsigned int i = 0; i < 256; i++)
		{
			if(bootHandle[i] == 0 && bootTickList[i] <= currentTick)
			{
				bootHandle[i] = 1;

				// Activate the interrupter.
				interrupterStateList[i] = 1;
				hasUpdateInterrupter = 1;
			}
		}

		if(hasUpdateInterrupter)
			InterrupterModule_UpdateState();

		// Check if the boot sequence is all done.
		unsigned char bootSequenceIsFinish = 1;
		for(unsigned int i = 0; i < 256; i++)
		{
			if(bootHandle[i] == 0)
			{
				bootSequenceIsFinish = 0;
				break;
			}
		}

		if(bootSequenceIsFinish)
		{
			// Kill the task.
			vTaskDelete(0);
		}
	}
}
