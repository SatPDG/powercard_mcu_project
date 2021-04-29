/*
 * main.c
 *
 *  Created on: Jan. 9, 2021
 *      Author: Leo Clouet
 *
 *  The main of this program is in this file. The OS and the tasks are started here.
 */

#include "directives.h"

#include <FreeRTOS.h>
#include <task.h>
#include "board.h"
#include "pin_mux.h"

#include "ledModule.h"
#include "adcDriver.h"

#include "samplingModule.h"

#include "serialTask.h"
#include "communicationModule.h"

#include "flashDriver.h"
#include "parameterSystem.h"

#include "interrupterModule.h"
#include "protectionModule.h"

#include "bootModule.h"

#include "segmentModule.h"

#include "watchdogModule.h"

void main() {

#ifndef DEBUG
	// Init the watchdog.
	{
		Watchdog_Init();
	}
#endif

	// Init the MCU principal pins and clock
	BOARD_ConfigMPU();
	BOARD_InitPins();
	BOARD_BootClockRUN();

	Serial_Init();

	ADCDriver_Init();

	SamplingModule_Init();

	//Init flash
	FlashDriver_Init();

	// Load the param from FLASH. Must be call once the flash is initialize.
	ParameterSystem_Init();

	// Init the interrupter
	InterrupterModule_Init();

	// Init the protection
	ProtectionModule_Init();

	// Init the 8 segments
	SegmentModule_Init();

	if (xTaskCreate(LedModule_Task, "LedTask", configMINIMAL_STACK_SIZE + 50, NULL, configMAX_PRIORITIES - 5, NULL) != pdTRUE) {
		while (1);
	}

	if (xTaskCreate(Serial_RXTask, "serialRXTask", configMINIMAL_STACK_SIZE + 200, NULL, configMAX_PRIORITIES, NULL) != pdTRUE) {
		while (1);
	}

	if (xTaskCreate(Serial_TXTask, "serialTXTask", configMINIMAL_STACK_SIZE + 200, NULL, configMAX_PRIORITIES, NULL) != pdTRUE) {
		while (1);
	}

	if (xTaskCreate(Communication_Task, "communicationTask", configMINIMAL_STACK_SIZE + 200, NULL, configMAX_PRIORITIES, NULL) != pdTRUE) {
		while (1);
	}

	if (xTaskCreate(SamplingModule_Task, "SamplingTask", configMINIMAL_STACK_SIZE + 100, NULL, configMAX_PRIORITIES - 2, NULL) != pdTRUE) {
		while (1);
	}

	if (xTaskCreate(ProtectionModule_Task, "ProtectionTask", configMINIMAL_STACK_SIZE + 50, NULL, configMAX_PRIORITIES - 2, NULL) != pdTRUE) {
			while (1);
	}

	if(xTaskCreate(BootModule_Task, "BootTask", configMINIMAL_STACK_SIZE + 50, NULL, configMAX_PRIORITIES - 3, NULL) != pdTRUE) {
		while (1);
	}

	if(xTaskCreate(InterrupterModule_AutoResetTask, "AutoResetTask", configMINIMAL_STACK_SIZE + 50, NULL, configMAX_PRIORITIES - 1, NULL) != pdTRUE) {
			while (1);
	}

	// Start the adc chain.
	ADCDriver_StartSampling();

	// Start the OS.
	vTaskStartScheduler();
	while (1);
}

