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

#include "uart1Driver.h"
#include "communicationModule.h"

#include "flashDriver.h"
#include "parameterSystem.h"

#include "interrupterModule.h"
#include "protectionModule.h"

void main() {
	BOARD_ConfigMPU();
	BOARD_InitPins();
	BOARD_BootClockRUN();

	ADCDriver_Init();

	SamplingModule_Init();

	UART1Driver_Init();

	//Init flash
	FlashDriver_Init();

	// Load the param from FLASH. Must be call once the flash is initialize.
	ParameterSystem_Init();

	// Init the interrupter
	InterrupterModule_Init();

	// Init the protection
	ProtectionModule_Init();

	if (xTaskCreate(LedModule_Task, "LedTask", configMINIMAL_STACK_SIZE + 50, NULL, configMAX_PRIORITIES - 3, NULL) != pdTRUE) {
		while (1);
	}

	if (xTaskCreate(UART1Driver_RXTask, "serialRXTask", configMINIMAL_STACK_SIZE + 200, NULL, configMAX_PRIORITIES, NULL) != pdTRUE) {
		while (1);
	}

	if (xTaskCreate(UART1Driver_TXTask, "serialTXTask", configMINIMAL_STACK_SIZE + 200, NULL, configMAX_PRIORITIES, NULL) != pdTRUE) {
		while (1);
	}

	if (xTaskCreate(Communication_Task, "communicationTask", configMINIMAL_STACK_SIZE + 200, NULL, configMAX_PRIORITIES, NULL) != pdTRUE) {
		while (1);
	}

	if (xTaskCreate(SamplingModule_Task, "SamplingTask", configMINIMAL_STACK_SIZE + 100, NULL, configMAX_PRIORITIES - 2, NULL) != pdTRUE) {
		while (1);
	}

	if (xTaskCreate(ProtectionModule_Task, "ProtectionTask", configMINIMAL_STACK_SIZE + 100, NULL, configMAX_PRIORITIES - 2, NULL) != pdTRUE) {
			while (1);
	}

	ADCDriver_StartSampling();

	vTaskStartScheduler();
	while (1);
}
