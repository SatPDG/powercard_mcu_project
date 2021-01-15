/*
 * ledModule.c
 *
 *  Created on: Jan. 9, 2021
 *      Author: Leo Clouet
 */

#include "ledModule.h"
#include "directives.h"

#include "fsl_iomuxc.h"
#include "fsl_gpio.h"

#include "FreeRTOS.h"
#include "task.h"

#ifdef EVK_1020
#define LED_PORT IOMUXC_GPIO_AD_B0_05_GPIO1_IO05
#define LED_GPIO GPIO1
#define LED_GPIO_PIN 5
#else

#endif

unsigned int ledStatus = 0;

void LedModule_Init() {
	IOMUXC_SetPinMux(LED_PORT, 0U);
	IOMUXC_SetPinConfig(LED_PORT, 0x10B0U);

	gpio_pin_config_t gpio_config = { .direction = kGPIO_DigitalOutput,
			.outputLogic = 0U, .interruptMode = kGPIO_NoIntmode };

	GPIO_PinInit(LED_GPIO, LED_GPIO_PIN, &gpio_config);
}

void LedModule_Task() {
	LedModule_Init();

	while (1) {
		vTaskDelay(pdMS_TO_TICKS(500));

		if(ledStatus == 0){
			GPIO_PinWrite(LED_GPIO, LED_GPIO_PIN, 0);
			ledStatus = 1;
		}else{
			GPIO_PinWrite(LED_GPIO, LED_GPIO_PIN, 1);
			ledStatus = 0;
		}
	}
}
