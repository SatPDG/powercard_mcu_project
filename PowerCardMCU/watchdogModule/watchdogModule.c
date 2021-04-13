/*
 * watchdogModule.c
 *
 *  Created on: Apr. 13, 2021
 *      Author: Leo Clouet
 *
 *  This module manage the watchdog. If the watchdog occur, all the interrupter are closed.
 */

#include "watchdogModule.h"

#include "fsl_wdog.h"
#include "interrupterModule.h"

void Watchdog_Init()
{
    wdog_config_t config;
    WDOG_GetDefaultConfig(&config);
    config.timeoutValue = 0xFU; 		// Timeout value is (0xF + 1)/2 = 8 sec.
    config.interruptTimeValue = 0x1U; 	// Interrupt occurred (0x1)/2 = 0.5 sec before WDOG timeout.
    config.enableInterrupt = 0x1;
    WDOG_Init(WDOG1, &config);

    NVIC_EnableIRQ(WDOG1_IRQn);
}

void Watchdog_Feed()
{
	WDOG_Refresh(WDOG1);
}

void WDOG1_IRQHandler()
{
	WDOG_ClearInterruptStatus(WDOG1, kWDOG_InterruptFlag);

	// Close all the interrupter.
	InterrupterModule_CloseAllInterrupter();
}
