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

void main() {
	BOARD_ConfigMPU();
	BOARD_InitPins();
	BOARD_BootClockRUN();

	if (xTaskCreate(LedModule_Task, "LedTask", configMINIMAL_STACK_SIZE + 50,
			NULL, configMAX_PRIORITIES - 3, NULL) != pdTRUE) {
		while(1);
	}

	vTaskStartScheduler();
	while(1);
}

