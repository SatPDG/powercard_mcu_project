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

#define PROTECTION_NBR_OF_PROTECTION 16

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

// This list hold the link between the protection output and the GPIO.
const protectionConfig_t protectionStateConfigList[PROTECTION_NBR_OF_PROTECTION] =
{
		{GPIO1, 3, IOMUXC_GPIO_AD_B0_03_GPIO1_IO03}, // Ethernet
		{GPIO1, 9, IOMUXC_GPIO_AD_B0_09_GPIO1_IO09}, // Mobile
		{GPIO1, 5, IOMUXC_GPIO_AD_B0_05_GPIO1_IO05}, // Lidar
		{GPIO1, 8, IOMUXC_GPIO_AD_B0_08_GPIO1_IO08}, // Camera
		{GPIO1, 4, IOMUXC_GPIO_AD_B0_04_GPIO1_IO04}, // Jetson
		{GPIO1, 19, IOMUXC_GPIO_AD_B1_03_GPIO1_IO19}, // 12v output 1
		{GPIO1, 18, IOMUXC_GPIO_AD_B1_02_GPIO1_IO18}, // 12v output 2
		{GPIO1, 17, IOMUXC_GPIO_AD_B1_01_GPIO1_IO17}, // 12v output 3
		{GPIO1, 16, IOMUXC_GPIO_AD_B1_00_GPIO1_IO16}, // 12v output 4
		{GPIO1, 27, IOMUXC_GPIO_AD_B1_11_GPIO1_IO27}, // Led 1
		{GPIO1, 28, IOMUXC_GPIO_AD_B1_12_GPIO1_IO28}, // Led 2
		{GPIO1, 10, IOMUXC_GPIO_AD_B0_10_GPIO1_IO10}, // Wifi
		{GPIO1, 14, IOMUXC_GPIO_AD_B0_14_GPIO1_IO14}, // 5v output 1
		{GPIO1, 13, IOMUXC_GPIO_AD_B0_13_GPIO1_IO13}, // 5v output 2
		{GPIO1, 12, IOMUXC_GPIO_AD_B0_12_GPIO1_IO12}, // 5v output 3
		{GPIO1, 11, IOMUXC_GPIO_AD_B0_11_GPIO1_IO11}, // 5v output 4
};

unsigned int protectionStateList[256];

void ProtectionModule_Init()
{
	// Init all the gpio
	for (unsigned int i = 0; i < PROTECTION_NBR_OF_PROTECTION; i++)
	{
		const protectionConfig_t *stateConfig = &protectionStateConfigList[i];

		IOMUXC_SetPinMux(stateConfig->muxRegister, stateConfig->muxMode,
				stateConfig->inputRegister, stateConfig->inputDaisy,
				stateConfig->configRegister, 0);
		IOMUXC_SetPinConfig(stateConfig->muxRegister, stateConfig->muxMode,
				stateConfig->inputRegister, stateConfig->inputDaisy,
				stateConfig->configRegister, 0x10B0U);

		gpio_pin_config_t gpioConfig;
		gpioConfig.direction = kGPIO_DigitalInput;
		gpioConfig.interruptMode = kGPIO_NoIntmode;
		gpioConfig.outputLogic = 0;
		GPIO_PinInit(stateConfig->gpioPort, stateConfig->gpioPin, &gpioConfig);

		// Read the value to init the protection status.
		if(GPIO_PinRead(stateConfig->gpioPort, stateConfig->gpioPin))
		{
			protectionStateList[i] = PROTECTION_OK;
		}
		else
		{
			protectionStateList[i] = PROTECTION_NO_CURRENT;
		}
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

			unsigned int state = GPIO_PinRead(stateConfig->gpioPort, stateConfig->gpioPin);
			protectionStateList[i] = (state == 0)? PROTECTION_NO_CURRENT : PROTECTION_OK;
		}
	}
}
