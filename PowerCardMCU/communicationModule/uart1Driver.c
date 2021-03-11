/*
 * uart1Driver.c
 *
 *  Created on: Jan. 17, 2021
 *      Author: Leo Clouet
 */

#include "uart1Driver.h"

#include "serialTask.h"
#include "fsl_lpuart.h"
#include "board.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#define UART1_RX_BUF_SIZE 300
#define UART1_TX_BUF_SIZE 300

// Driver attribute
unsigned char uart1RXBuffer[UART1_RX_BUF_SIZE];
unsigned char uart1TXBuffer[UART1_TX_BUF_SIZE];

unsigned int uart1RXIdx = 0;
unsigned int uart1RXLastRead = 0;
unsigned int uart1TXIdx = 0;
unsigned int uart1TXLastWrite = 0;

void UART1Driver_Init()
{
	// Init uart peripheral
	{
		lpuart_config_t config;

		LPUART_GetDefaultConfig(&config);
		config.baudRate_Bps = 115200;
		config.enableTx = true;
		config.enableRx = true;
		LPUART_Init(LPUART1, &config, BOARD_DebugConsoleSrcFreq());

		LPUART_EnableInterrupts(LPUART1, kLPUART_RxDataRegFullInterruptEnable);
		NVIC_SetPriority(LPUART1_IRQn, 2);
		EnableIRQ(LPUART1_IRQn);
	}
}

int UART1Driver_Read(unsigned char *buffer, unsigned int size)
{
	unsigned int count = 0;
	while (uart1RXLastRead != uart1RXIdx)
	{
		buffer[count] = uart1RXBuffer[uart1RXLastRead];
		uart1RXLastRead = (uart1RXLastRead + 1) % UART1_RX_BUF_SIZE;
		count++;
	}
	return count;
}

int UART1Driver_Write(unsigned char *buffer, unsigned int size)
{
	LPUART_DisableInterrupts(LPUART1,
			kLPUART_TxDataRegEmptyInterruptEnable);

	for (unsigned int i = 0; i < size; i++)
	{
		uart1TXBuffer[uart1TXIdx] = buffer[i];
		uart1TXIdx = (uart1TXIdx + 1) % UART1_TX_BUF_SIZE;
	}

	LPUART_WriteByte(LPUART1, uart1TXBuffer[uart1TXLastWrite]);
	uart1TXLastWrite = (uart1TXLastWrite + 1) % UART1_TX_BUF_SIZE;

	LPUART_EnableInterrupts(LPUART1,
			kLPUART_TxDataRegEmptyInterruptEnable);
	return size;
}

void LPUART1_IRQHandler()
{
	unsigned int flags = LPUART_GetStatusFlags(LPUART1);

	if (flags & kLPUART_RxDataRegFullFlag)
	{
		Serial_StartInterbyteTimer();
		uart1RXBuffer[uart1RXIdx] = LPUART_ReadByte(LPUART1);
		uart1RXIdx = (uart1RXIdx + 1) % UART1_RX_BUF_SIZE;
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
