/*
 * samplingModule.c
 *
 *  Created on: Jan. 16, 2021
 *      Author: Leo Clouet
 */

#include "samplingModule.h"

#include "FreeRTOS.h"
#include "semphr.h"

unsigned int voltageAccumulate = 0;
unsigned int currentAccumulate = 0;
unsigned int temperatureAccumulate = 0;

unsigned int nbrVoltageSample = 0;
unsigned int nbrCurrentSample = 0;
unsigned int nbrTemperatureSample = 0;

float voltage;
float current;
float temperature;

SemaphoreHandle_t sampling_sem;

void SamplingModule_Init()
{
	sampling_sem = xSemaphoreCreateBinary();
	xSemaphoreGive(sampling_sem);
}

void SamplingModule_Task()
{
	while (1)
	{
		vTaskDelay(pdMS_TO_TICKS(100));

		if (xSemaphoreTake(sampling_sem, portMAX_DELAY))
		{

			voltage = (float) voltageAccumulate / (float) nbrVoltageSample;
			current = (float) currentAccumulate / (float) nbrCurrentSample;
			temperature = (float) temperatureAccumulate
					/ (float) nbrTemperatureSample;

			voltageAccumulate = 0;
			currentAccumulate = 0;
			temperatureAccumulate = 0;
			nbrVoltageSample = 0;
			nbrCurrentSample = 0;
			nbrTemperatureSample = 0;

			xSemaphoreGive(sampling_sem);
		}
	}
}

void SamplingModule_PushSamples(unsigned short voltage, unsigned short current,
		unsigned short temperature)
{
	long pxHigherPriorityTaskWoken;
	if (xSemaphoreTakeFromISR(sampling_sem,
			&pxHigherPriorityTaskWoken) == pdTRUE)
	{

		voltageAccumulate += voltage;
		nbrVoltageSample++;
		currentAccumulate += current;
		nbrCurrentSample++;
		temperatureAccumulate += temperature;
		nbrTemperatureSample++;

		xSemaphoreGiveFromISR(sampling_sem, &pxHigherPriorityTaskWoken);
	}
}
