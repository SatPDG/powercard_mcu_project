/*
 * uart1Driver.c
 *
 *  Created on: Jan. 17, 2021
 *      Author: Leo Clouet
 */

#include "uart1Driver.h"

#include "fsl_lpuart.h"
#include "board.h"
#include "fsl_gpt.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "communicationParser.h"
#include "communicationModule.h"

#define UART1_RX_BUF_SIZE 300
#define UART1_TX_BUF_SIZE 300

// Driver attribute
unsigned char uart1RXBuffer[UART1_RX_BUF_SIZE];
unsigned char uart1TXBuffer[UART1_TX_BUF_SIZE];

unsigned int uart1RXIdx = 0;
unsigned int uart1RXLastRead = 0;
unsigned int uart1TXIdx = 0;
unsigned int uart1TXLastWrite = 0;

// Rx task attributs
SemaphoreHandle_t uart1RX_sem;

// Tx task attributs
QueueHandle_t uart1TX_queue;

void UART1Driver_Init()
{
	uart1RX_sem = xSemaphoreCreateBinary();
	uart1TX_queue = xQueueCreate(2, sizeof(comData_t));

	CommunicationParser_Init();

	// Init uart peripheral
	{
		lpuart_config_t config;

		LPUART_GetDefaultConfig(&config);
		config.baudRate_Bps = 115200;
		config.enableTx = true;
		config.enableRx = true;
		LPUART_Init(LPUART1, &config, BOARD_DebugConsoleSrcFreq());

		LPUART_EnableInterrupts(LPUART1, kLPUART_RxDataRegFullInterruptEnable);
		EnableIRQ(LPUART1_IRQn);
	}

	// Init timer peripheral
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
		EnableIRQ(GPT2_IRQn);
	}
}

void LPUART1_IRQHandler()
{
	unsigned int flags = LPUART_GetStatusFlags(LPUART1);

	if (flags & kLPUART_RxDataRegFullFlag)
	{
		GPT_StopTimer(GPT2);
		GPT_StartTimer(GPT2);
		uart1RXBuffer[uart1RXIdx] = LPUART_ReadByte(LPUART1);
		uart1RXIdx = (uart1RXIdx + 1) % UART1_RX_BUF_SIZE;

		long priority;
		xSemaphoreGiveFromISR(uart1RX_sem, &priority);
	}
	else if (flags & kLPUART_TxDataRegEmptyFlag)
	{
		if (uart1TXIdx != uart1TXLastWrite)
		{
			LPUART_WriteByte(LPUART1, uart1TXBuffer[uart1TXLastWrite]);
			uart1TXLastWrite = (uart1TXLastWrite + 1) % UART1_TX_BUF_SIZE;
		}
		else
		{
			LPUART_DisableInterrupts(LPUART1,
					kLPUART_TxDataRegEmptyInterruptEnable);
		}
	}
}

void GPT2_IRQHandler()
{
	GPT_ClearStatusFlags(GPT2, kGPT_OutputCompare1Flag);
	GPT_StopTimer(GPT2);

	CommunicationParser_Flush();
}

void UART1Driver_RXTask()
{
	unsigned char uartRXTmp[256];
	while (1)
	{
		if (xSemaphoreTake(uart1RX_sem, portMAX_DELAY) == pdTRUE)
		{
			unsigned int idx = 0;
			while (uart1RXLastRead != uart1RXIdx)
			{
				uartRXTmp[idx] = uart1RXBuffer[uart1RXLastRead];
				uart1RXLastRead = (uart1RXLastRead + 1) % UART1_RX_BUF_SIZE;
				idx++;
			}

			// Parse data
			unsigned int result = CommunicationParser_Parse(uartRXTmp, idx);
			if (result)
			{
				serialRxPacket_t message;
				message.packet = parser.packet;
				message.txQueue = uart1TX_queue;

				parser.packet.data = 0;

				xQueueSend(com_queue, &message, 0);
				CommunicationParser_Flush();
			}
		}
	}
}

void UART1Driver_TXTask()
{
	comData_t message;
	while (1)
	{
		if (xQueueReceive(uart1TX_queue, &message, portMAX_DELAY))
		{
			vTaskSuspendAll();
			LPUART_DisableInterrupts(LPUART1,
					kLPUART_TxDataRegEmptyInterruptEnable);

			for (unsigned int i = 0; i < message.size; i++)
			{
				uart1TXBuffer[uart1TXIdx] = message.data[i];
				uart1TXIdx = (uart1TXIdx + 1) % UART1_TX_BUF_SIZE;
			}

			LPUART_WriteByte(LPUART1, uart1TXBuffer[uart1TXLastWrite]);

			LPUART_EnableInterrupts(LPUART1,
					kLPUART_TxDataRegEmptyInterruptEnable);
			xTaskResumeAll();
		}
	}
}

