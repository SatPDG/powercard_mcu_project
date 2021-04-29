/*
 * bootModule.c
 *
 *  Created on: Mar. 16, 2021
 *      Author: Leo Clouet
 *
 *  This module enable the interrupter for each peripheral when a delay is meet for each interrupter.
 *  If the value in the boot time list is NO_BOOT, the interrupter will not be enable.
 */

#include "bootModule.h"

#include "FreeRTOS.h"
#include "task.h"
#include "interrupterModule.h"

#define NO_BOOT 0xFFFFFFFF

unsigned int bootTimeList[256];
const unsigned int bootTimeDefaultList[256] =
{
	0, // Ethernet
	0, // Mobile
	0, // Lidar
	0, // Camera
	0, // Jetson
	0, // 12v output 1
	0, // 12v output 2
	0, // 12v output 3
	0, // 12v ouptut 4
	0, // Led 1
	0, // Led 2
	0, // Wifi
	0, // 5v output 1
	0, // 5v output 2
	0, // 5v output 3
	0, // 5v output 4
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
		if(bootTimeList[i] == NO_BOOT)
		{
			bootHandle[i] = 1;
		}
		else
		{
			bootHandle[i] = 0;
			bootTickList[i] = pdMS_TO_TICKS(bootTimeList[i]);
		}
	}


	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(200));
		unsigned int currentTick = xTaskGetTickCount();

		for(unsigned int i = 0; i < 256; i++)
		{
			if(bootHandle[i] == 0 && bootTickList[i] <= currentTick)
			{
				bootHandle[i] = 1;

				// Activate the interrupter.
				InterrupterModule_UpdateInterrupterState(i, INTERRUPTER_OPEN);
			}
		}

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
			while(1);
		}
	}
}
