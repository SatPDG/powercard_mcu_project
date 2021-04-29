/*
 * segmentModule.c
 *
 *  Created on: Feb. 16, 2021
 *      Author: Leo Clouet
 *
 *  This module manage the 8 segments.
 *  It takes the voltage sampling lecture and display it on the 8 segments.
 *
 *  This code has not been test.
 */

#include "segmentModule.h"

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_pit.h"

typedef struct
{
	GPIO_Type *gpioPort;
	unsigned int gpioPin;
	unsigned int muxRegister;
	unsigned int muxMode;
	unsigned int inputRegister;
	unsigned int inputDaisy;
	unsigned int configRegister;
} segmentGpio_t;

#define SEGMENT_SEGA 0
#define SEGMENT_SEGB 1
#define SEGMENT_SEGC 2
#define SEGMENT_SEGD 3
#define SEGMENT_SEGE 4
#define SEGMENT_SEGF 5
#define SEGMENT_SEGG 6
#define SEGMENT_SEGH 7
#define SEGMENT_POINT 8

/**
 *           A
 * 			***
 *         *   *
 *      F  *   * B
 *         * G *
 *          ***
 *         *   *
 *      E  *   * C
 *         *   *
 *          ***
 *           D
 */

// This table hold which gpio to set to print the corresponding digit.
const unsigned char characterTable[10][7] =
{
		{ 1, 1, 1, 1, 1, 1, 0 }, // Digit 0
		{ 0, 1, 1, 0, 0, 0, 0 }, // Digit 1
		{ 1, 1, 0, 1, 1, 0, 1 }, // Digit 2
		{ 1, 1, 1, 1, 0, 0, 1 }, // Digit 3
		{ 0, 1, 1, 0, 0, 1, 1 }, // Digit 4
		{ 1, 0, 1, 1, 0, 1, 1 }, // Digit 5
		{ 1, 0, 1, 1, 1, 1, 1 }, // Digit 6
		{ 1, 1, 1, 0, 0, 0, 0 }, // Digit 7
		{ 1, 1, 1, 1, 1, 1, 1 }, // Digit 8
		{ 1, 1, 1, 1, 0, 1, 1 }, // Digit 9
};

// This list hold the gpio of the 8 segments for the 8 segments.
const segmentGpio_t segmentList[7];
// This list hold the gpio of the 8 segments for the 3 digit.
const segmentGpio_t digitList[3];

unsigned int digitType[3];
unsigned int segmentUpdateIdx = 0;

void SegmentModule_Init()
{
	// Init all the gpio.
	for (unsigned int i = 0; i < 9; i++)
	{
		const segmentGpio_t *config = &segmentList[i];

		IOMUXC_SetPinMux(config->muxRegister, config->muxMode,
				config->inputRegister, config->inputDaisy,
				config->configRegister, 0);
		IOMUXC_SetPinConfig(config->muxRegister, config->muxMode,
				config->inputRegister, config->inputDaisy,
				config->configRegister, 0x10B0U);

		gpio_pin_config_t gpioConfig;
		gpioConfig.direction = kGPIO_DigitalOutput;
		gpioConfig.interruptMode = kGPIO_NoIntmode;
		gpioConfig.outputLogic = 0;
		GPIO_PinInit(config->gpioPort, config->gpioPin, &gpioConfig);
	}
	for (unsigned int i = 0; i < 3; i++)
	{
		const segmentGpio_t *config = &digitList[i];

		IOMUXC_SetPinMux(config->muxRegister, config->muxMode,
				config->inputRegister, config->inputDaisy,
				config->configRegister, 0);
		IOMUXC_SetPinConfig(config->muxRegister, config->muxMode,
				config->inputRegister, config->inputDaisy,
				config->configRegister, 0x10B0U);

		gpio_pin_config_t gpioConfig;
		gpioConfig.direction = kGPIO_DigitalOutput;
		gpioConfig.interruptMode = kGPIO_NoIntmode;
		gpioConfig.outputLogic = 1;
		GPIO_PinInit(config->gpioPort, config->gpioPin, &gpioConfig);
	}

	// Init the PIT so the number can be refresh at a certain frequency.
	// The PIT is init in the sampling module.
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, (24000000 / 9) - 1);
	PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
    NVIC_EnableIRQ(PIT_IRQn);
}

void PIT_IRQHandler()
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);

	// Raise the voltage of the last digit.
	{
		unsigned int oldIdx = (segmentUpdateIdx - 1) % 3;
		GPIO_PinWrite(digitList[oldIdx].gpioPort, digitList[oldIdx].gpioPin, 1);
	}

	// Update the pin of the current digit.
	for(unsigned int i = 0; i < 7; i++)
	{
		const segmentGpio_t *segment = &segmentList[i];
		GPIO_PinWrite(segment->gpioPort, segment->gpioPin, characterTable[digitType[segmentUpdateIdx]][i]);
	}

	// Lower the voltage of the current digit.
	GPIO_PinWrite(digitList[segmentUpdateIdx].gpioPort, digitList[segmentUpdateIdx].gpioPin, 0);

	segmentUpdateIdx = (segmentUpdateIdx + 1) % 3;
}

/*
 * This function take a float value and decompose it so the
 * 3 first digit of the number can be display on the 8 segments.
 */
void SegmentModule_UpdateDisplayValue(float val)
{
	int tmp = (int) (val * 10);

	digitType[2] = tmp % 10;
	digitType[1] = (tmp % 100) - digitType[2];
	digitType[0] = (tmp % 1000) - digitType[2] - digitType[1];
}
