/*
 * interrupterModule.c
 *
 *  Created on: Feb. 16, 2021
 *      Author: Leo Clouet
 *
 *  This module, deal with the interrupter.
 *  Since in the last design, the interrupter are in fact the enable pin of the efuse,
 *  this module is link with the protection module.
 *
 *  The auto reset task, make sure that if a efuse blow, the efuse is enable not long after by the MCU.
 */

#include "interrupterModule.h"

#include "protectionModule.h"

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#include "FreeRTOS.h"
#include "task.h"

#define INTERRUPTER_NBR_OF_INTERRUPTER 2

typedef struct
{
	GPIO_Type *gpioPort;
	unsigned int gpioPin;
	unsigned int muxRegister;
	unsigned int muxMode;
	unsigned int inputRegister;
	unsigned int inputDaisy;
	unsigned int configRegister;
} interrupterConfig_t;

unsigned int interrupterStateList[256];
// The list that link the interrupter and it's GPIO.
const interrupterConfig_t interrupterConfigList[INTERRUPTER_NBR_OF_INTERRUPTER] =
{
		{GPIO1, 25, IOMUXC_GPIO_AD_B1_09_GPIO1_IO25},
		{GPIO1, 24, IOMUXC_GPIO_AD_B1_08_GPIO1_IO24},
};

unsigned int autoResetInterruptList[256];
const unsigned int autoResetDefaultList[256] =
{
	0
};

void InterrupterModule_Init()
{
	// Init the GPIO for each interrupter.
	for(unsigned int i = 0; i < INTERRUPTER_NBR_OF_INTERRUPTER; i++)
	{
		const interrupterConfig_t *config = &interrupterConfigList[i];

		IOMUXC_SetPinMux(config->muxRegister, config->muxMode, config->inputRegister, config->inputDaisy, config->configRegister, 0);
		IOMUXC_SetPinConfig(config->muxRegister, config->muxMode, config->inputRegister, config->inputDaisy, config->configRegister, 0x10B0U);

		gpio_pin_config_t gpioConfig;
		gpioConfig.direction = kGPIO_DigitalOutput;
		gpioConfig.interruptMode = kGPIO_NoIntmode;
		gpioConfig.outputLogic = 0;
		GPIO_PinInit(config->gpioPort, config->gpioPin, &gpioConfig);

		// Set the interrupter to low.
		GPIO_PinWrite(config->gpioPort, config->gpioPin, 0x0);
	}
}

unsigned int InterrupterModule_SetInterrupters(unsigned int offset, unsigned int count, unsigned char *data)
{
	// Set the value of the list of interrupter sent in parameters.
	unsigned int *interrupterPtr = (unsigned int*) data;
	for(unsigned int i = 0; i < count; i++)
	{
		unsigned int interrupterValue = interrupterPtr[i];
		// Update the GPIO link to the interrupter.
		InterrupterModule_UpdateInterrupterState(offset + i, interrupterValue);
	}
	return 0x1;
}

void InterrupterModule_UpdateInterrupterState(unsigned int interrupter, unsigned int state)
{
	// If the interrupter exist.
	if(interrupter < INTERRUPTER_NBR_OF_INTERRUPTER){
		// If we want to close the interrupter
		if (state == INTERRUPTER_CLOSE)
		{
			// If the interrupter is open, close it.
			if (interrupterStateList[interrupter] == INTERRUPTER_OPEN)
			{
				interrupterStateList[interrupter] = INTERRUPTER_CLOSE;
				const interrupterConfig_t *config =
						&interrupterConfigList[interrupter];
				GPIO_PinWrite(config->gpioPort, config->gpioPin, 0x0);
			}
		}
		else if (state == INTERRUPTER_OPEN)
		{
			// If the interrupter is close, open it.
			if (interrupterStateList[interrupter] == INTERRUPTER_CLOSE)
			{
				interrupterStateList[interrupter] = INTERRUPTER_OPEN;
				const interrupterConfig_t *config =
						&interrupterConfigList[interrupter];
				GPIO_PinWrite(config->gpioPort, config->gpioPin, 0x1);
			}
		}
		else if (state == INTERRUPTER_RESET)
		{
			// If the interrupter is close, just open it.
			if (interrupterStateList[interrupter] == INTERRUPTER_CLOSE)
			{
				interrupterStateList[interrupter] = INTERRUPTER_OPEN;
				const interrupterConfig_t *config =
						&interrupterConfigList[interrupter];
				GPIO_PinWrite(config->gpioPort, config->gpioPin, 0x1);
			}
			// If the interrupter is open, close and open it again.
			else
			{
				const interrupterConfig_t *config =
						&interrupterConfigList[interrupter];
				// Close the interrupter.
				GPIO_PinWrite(config->gpioPort, config->gpioPin, 0x0);
				// Wait some time.
				for (unsigned int j = 0; j < 100000; j++);
				// Open the interrupter.
				GPIO_PinWrite(config->gpioPort, config->gpioPin, 0x1);
			}
		}
	}
}

void InterrupterModule_AutoResetTask()
{
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(500));

		// Check for all the interrupter.
		for(unsigned int i = 0; i < INTERRUPTER_NBR_OF_INTERRUPTER; i++)
		{
			// If the interrupter is in the auto reset list.
			if(autoResetInterruptList[i] == 0x01)
			{
				// Check if the protection has cut and the interrupter is open.
				if(protectionStateList[i] == PROTECTION_NO_CURRENT && interrupterStateList[i] == INTERRUPTER_OPEN)
				{
					// Reset the interrupter.
					InterrupterModule_UpdateInterrupterState(i, INTERRUPTER_RESET);
				}
			}
		}
	}
}
