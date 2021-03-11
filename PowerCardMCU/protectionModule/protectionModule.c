/*
 * protectionModule.c
 *
 *  Created on: Feb. 16, 2021
 *      Author: Leo Clouet
 */

#include "protectionModule.h"

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#include "FreeRTOS.h"
#include "task.h"

#define PROTECTION_NBR_OF_PROTECTION 2

typedef struct
{
	GPIO_Type *gpioPort;
	unsigned int gpioPin;
	unsigned int muxRegister;
	unsigned int muxMode;
	unsigned int inputRegister;
	unsigned int inputDaisy;
	unsigned int configRegister;
} protectionConfig_t;

const protectionConfig_t protectionStateConfigList[PROTECTION_NBR_OF_PROTECTION] =
{
{ GPIO1, 0, IOMUXC_GPIO_AD_B0_00_GPIO1_IO00 },
{ GPIO1, 0, IOMUXC_GPIO_AD_B0_00_GPIO1_IO00 }, };

const protectionConfig_t protectionOverCurrentConfigList[PROTECTION_NBR_OF_PROTECTION] =
{
{ GPIO1, 0, IOMUXC_GPIO_AD_B0_00_GPIO1_IO00 },
{ GPIO1, 0, IOMUXC_GPIO_AD_B0_00_GPIO1_IO00 }, };

unsigned int protectionStateList[256];
unsigned int protectionOverCurrentList[256];

void ProtectionModule_Init()
{
	// Init all the gpio
	for (unsigned int i = 0; i < PROTECTION_NBR_OF_PROTECTION; i++)
	{
		const protectionConfig_t *stateConfig = &protectionStateConfigList[i];
		const protectionConfig_t *currentConfig = &protectionOverCurrentConfigList[i];

		IOMUXC_SetPinMux(stateConfig->muxRegister, stateConfig->muxMode,
				stateConfig->inputRegister, stateConfig->inputDaisy,
				stateConfig->configRegister, 0);
		IOMUXC_SetPinConfig(stateConfig->muxRegister, stateConfig->muxMode,
				stateConfig->inputRegister, stateConfig->inputDaisy,
				stateConfig->configRegister, 0x10B0U);

		IOMUXC_SetPinMux(currentConfig->muxRegister, currentConfig->muxMode,
				currentConfig->inputRegister, currentConfig->inputDaisy,
				currentConfig->configRegister, 0);
		IOMUXC_SetPinConfig(currentConfig->muxRegister, currentConfig->muxMode,
				currentConfig->inputRegister, currentConfig->inputDaisy,
				currentConfig->configRegister, 0x10B0U);

		gpio_pin_config_t gpioConfig;
		gpioConfig.direction = kGPIO_DigitalInput;
		gpioConfig.interruptMode = kGPIO_NoIntmode;
		gpioConfig.outputLogic = 0;
		GPIO_PinInit(stateConfig->gpioPort, stateConfig->gpioPin, &gpioConfig);
		GPIO_PinInit(currentConfig->gpioPort, currentConfig->gpioPin,
				&gpioConfig);
	}
}

void ProtectionModule_Task()
{
	while (1)
	{
		vTaskDelay(pdMS_TO_TICKS(200));

		// Update all the protection status
		for (unsigned int i = 0; i < PROTECTION_NBR_OF_PROTECTION; i++)
		{
			const protectionConfig_t *stateConfig = &protectionStateConfigList[i];
			const protectionConfig_t *currentConfig = &protectionOverCurrentConfigList[i];

			 unsigned int state = GPIO_PinRead(stateConfig->gpioPort, stateConfig->gpioPin);
			 protectionStateList[i] = (state == 0)? 0 : 1;

			 state = GPIO_PinRead(currentConfig->gpioPort, currentConfig->gpioPin);
			 protectionOverCurrentList[i] = (state == 0)? 0 : 1;
		}
	}
}
