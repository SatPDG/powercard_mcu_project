/*
 * main.c
 *
 *  Created on: Jan. 9, 2021
 *      Author: Leo Clouet
 */

#include <FreeRTOS.h>
#include <task.h>
#include "board.h"
#include "pin_mux.h"

#include "ledModule.h"
#include "adcDriver.h"

#include "samplingModule.h"

void main()
{
	BOARD_ConfigMPU();
	BOARD_InitPins();
	BOARD_BootClockRUN();

	ADCDriver_Init();

	SamplingModule_Init();

	if (xTaskCreate(LedModule_Task, "LedTask", configMINIMAL_STACK_SIZE + 50,
	NULL, configMAX_PRIORITIES - 3, NULL) != pdTRUE)
	{
		while (1)
			;
	}
	if (xTaskCreate(SamplingModule_Task, "SamplingTask",
	configMINIMAL_STACK_SIZE + 100,
	NULL, configMAX_PRIORITIES - 1, NULL) != pdTRUE)
	{
		while (1)
			;
	}

	ADCDriver_StartSampling();

	vTaskStartScheduler();
	while (1)
		;
}

