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

#define INTERRUPTER_NBR_OF_INTERRUPTER 16

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
		{GPIO2, 8, IOMUXC_GPIO_EMC_08_GPIO2_IO08}, // Ethernet
		{GPIO2, 6, IOMUXC_GPIO_EMC_06_GPIO2_IO06}, // Mobile
		{GPIO2, 5, IOMUXC_GPIO_EMC_05_GPIO2_IO05}, // Lidar
		{GPIO2, 4, IOMUXC_GPIO_EMC_04_GPIO2_IO04}, // Camera
		{GPIO2, 3, IOMUXC_GPIO_EMC_03_GPIO2_IO03}, // Jetson
		{GPIO3, 15, IOMUXC_GPIO_SD_B0_02_GPIO3_IO15}, // 12v output 1
		{GPIO3, 16, IOMUXC_GPIO_SD_B0_03_GPIO3_IO16}, // 12v output 2
		{GPIO3, 18, IOMUXC_GPIO_SD_B0_05_GPIO3_IO18}, // 12v output 3
		{GPIO3, 19, IOMUXC_GPIO_SD_B0_06_GPIO3_IO19}, // 12v output 4
		{GPIO3, 13, IOMUXC_GPIO_SD_B0_00_GPIO3_IO13}, // Led 1
		{GPIO3, 14, IOMUXC_GPIO_SD_B0_01_GPIO3_IO14}, // Led 2
		{GPIO2, 9, IOMUXC_GPIO_EMC_09_GPIO2_IO09}, // Wifi
		{GPIO2, 10, IOMUXC_GPIO_EMC_10_GPIO2_IO10}, // 5v output 1
		{GPIO2, 11, IOMUXC_GPIO_EMC_11_GPIO2_IO11}, // 5v output 2
		{GPIO3, 7, IOMUXC_GPIO_EMC_39_GPIO3_IO07}, // 5v output 3
		{GPIO3, 6, IOMUXC_GPIO_EMC_38_GPIO3_IO06}, // 5v output 4
};

unsigned int autoResetInterruptList[256];
const unsigned int autoResetDefaultList[256] =
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

void InterrupterModule_CloseAllInterrupter()
{
	for(unsigned int i = 0; i < INTERRUPTER_NBR_OF_INTERRUPTER; i++)
	{
		InterrupterModule_UpdateInterrupterState(i, INTERRUPTER_CLOSE);
	}
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
