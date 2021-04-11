/*
 * uartTask.c
 *
 *  Created on: Mar. 11, 2021
 *      Author: Leo Clouet
 */

#include <serialTask.h>

#include "uart1Driver.h"
#include "usbDriver.h"

#include "communicationParser.h"
#include "communicationModule.h"

#include "fsl_gpt.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"

#define UART1_DRIVER_FLAG 0x1
#define USB_DRIVER_FLAG 0x2

// Parser info
comParser_t uart1Parser;
comParser_t usbParser;

// Rx task attributs
SemaphoreHandle_t serialRX_sem;

// Tx task attributs
QueueHandle_t serialTX_queue;

unsigned int timeoutFlag = 0;

void Serial_Init()
{
	serialRX_sem = xSemaphoreCreateBinary();
	serialTX_queue = xQueueCreate(2, sizeof(comData_t));

	// Init the parser from the uart and usb port.
	CommunicationParser_Init(&uart1Parser);
	CommunicationParser_Init(&usbParser);

	// Init the time peripheral. This timer is the interbyte timer.
	// When an event occur, that mean that a complete message has been sent.
	{
		gpt_config_t gptConfig;
		unsigned int gptFreq = 0;

		GPT_GetDefaultConfig(&gptConfig);
		GPT_Init(GPT2, &gptConfig);
		GPT_SetClockDivider(GPT2, 3);

		gptFreq = (CLOCK_GetFreq(kCLOCK_PerClk) / 2880) - 1;
		GPT_SetOutputCompareValue(GPT2, kGPT_OutputCompare_Channel1,
				gptFreq);
		GPT_EnableInterrupts(GPT2, kGPT_OutputCompare1InterruptEnable);
		NVIC_SetPriority(GPT2_IRQn, 2);
		EnableIRQ(GPT2_IRQn);
	}

	// Init uart drivers
	UART1Driver_Init();
	USBDriver_Init();
}

void Serial_StartInterbyteTimer()
{
	GPT_StopTimer(GPT2);
	GPT_StartTimer(GPT2);
}

void GPT2_IRQHandler()
{
	// When this IRQ occur, a complet message has been received in one of the uart.
	GPT_ClearStatusFlags(GPT2, kGPT_OutputCompare1Flag);
	GPT_StopTimer(GPT2);

	// Raise the serial rx semaphore to start the data collect process.
	timeoutFlag = 1;
	long priority;
	xSemaphoreGiveFromISR(serialRX_sem, &priority);
}

void Serial_RXTask()
{
	unsigned char rxBuffer[256];
	while (1)
	{
		if (xSemaphoreTake(serialRX_sem, portMAX_DELAY) == pdTRUE)
		{
			// Disable irq to take the right value of timeout flags.
			__disable_irq();
			unsigned int currentTimeout = timeoutFlag;
			timeoutFlag = 0;
			__enable_irq();

			// Check the uart1 driver.
			{
				// Fetch the data in the uart.
				unsigned int count = UART1Driver_Read(rxBuffer, 256);
				if(count  > 0)
				{
					// Parse the data.
					unsigned int result = CommunicationParser_Parse(&uart1Parser, rxBuffer, count);
					if (result)
					{
						// A valid message was found. Take it and send it to the communication task.
						serialRxPacket_t message;
						message.packet = uart1Parser.packet;
						message.txQueue = serialTX_queue;
						message.driverFlags = UART1_DRIVER_FLAG;

						// To evitate the free on the packet buffer.
						uart1Parser.packet.data = 0;
						xQueueSend(com_queue, &message, 0);
						CommunicationParser_Flush(&uart1Parser);
					}
				}
			}

			// Check the usb driver
			{
				// Fetch the data in the uart.
				unsigned int count = USBDriver_Read(rxBuffer, 256);
				if(count  > 0)
				{
					// Parse the data.
					unsigned int result = CommunicationParser_Parse(&usbParser, rxBuffer, count);
					if (result)
					{
						// A valid message was found. Take it and send it to the communication task.
						serialRxPacket_t message;
						message.packet = usbParser.packet;
						message.txQueue = serialTX_queue;
						message.driverFlags = USB_DRIVER_FLAG;

						// To evitate the free on the packet buffer.
						usbParser.packet.data = 0;
						xQueueSend(com_queue, &message, 0);
						CommunicationParser_Flush(&usbParser);
					}
				}
			}

			// Flush parsers if a timeout occur.
			if(currentTimeout)
			{
				CommunicationParser_Flush(&uart1Parser);
				CommunicationParser_Flush(&usbParser);
			}
		}
	}
}

void Serial_TXTask()
{
	comData_t message;
	while (1)
	{
		if (xQueueReceive(serialTX_queue, &message, portMAX_DELAY))
		{
			// Suspend the os, so the data is write the without interruption.
			// Also because the uart deactivate it's irq. So the OS must not be running.
			vTaskSuspendAll();

			// Select the right uart to send the data to.
			if(message.driverFlags == UART1_DRIVER_FLAG){
				UART1Driver_Write(message.data, message.size);
			}else if(message.driverFlags == USB_DRIVER_FLAG){
				USBDriver_Write(message.data, message.size);
			}

			xTaskResumeAll();

			// Free the data.
			vPortFree(message.data);
		}
	}
}
