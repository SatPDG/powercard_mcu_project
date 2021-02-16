/*
 * interrupterModule.c
 *
 *  Created on: Feb. 16, 2021
 *      Author: Leo Clouet
 */

#include "interrupterModule.h"

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

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

unsigned char interrupterStateList[256];
const interrupterConfig_t interrupterConfigList[INTERRUPTER_NBR_OF_INTERRUPTER] =
{
		{GPIO1, 0, IOMUXC_GPIO_AD_B0_00_GPIO1_IO00},
		{GPIO1, 0, IOMUXC_GPIO_AD_B0_00_GPIO1_IO00},
};

void InterrupterModule_Init()
{
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


		unsigned char output = (interrupterStateList[i] == 0) ? 0 : 1;
		GPIO_PinWrite(config->gpioPort, config->gpioPin, output);
	}
}

void InterrupterModule_UpdateState()
{
	for(unsigned int i = 0; i < INTERRUPTER_NBR_OF_INTERRUPTER; i++)
	{
		const interrupterConfig_t *config = &interrupterConfigList[i];

		unsigned char output = (interrupterStateList[i] == 0) ? 0 : 1;
		GPIO_PinWrite(config->gpioPort, config->gpioPin, output);
	}
}
