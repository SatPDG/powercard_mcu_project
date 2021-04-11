/*
 * uart1Driver.c
 *
 *  Created on: Jan. 17, 2021
 *      Author: Leo Clouet
 */

#include "uart1Driver.h"

#include "serialTask.h"
#include "fsl_lpuart.h"
#include "fsl_iomuxc.h"
#include "board.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#define UART1_RX_BUF_SIZE 512
#define UART1_TX_BUF_SIZE 512

// Driver attribute
unsigned char uart1RXBuffer[UART1_RX_BUF_SIZE];
unsigned char uart1TXBuffer[UART1_TX_BUF_SIZE];

unsigned int uart1RXIdx = 0;
unsigned int uart1RXLastRead = 0;
unsigned int uart1TXIdx = 0;
unsigned int uart1TXLastWrite = 0;

void UART1Driver_Init()
{
	// Init uart pin
	{
		 IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_06_LPUART1_TX, 0U);
		 IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_07_LPUART1_RX, 0U);
		 IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_06_LPUART1_TX, 0x10B0u);
		 IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_07_LPUART1_RX, 0x10B0u);
	}
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
		// Read all the received data since the last time the function was called.
		buffer[count] = uart1RXBuffer[uart1RXLastRead];
		uart1RXLastRead = (uart1RXLastRead + 1) % UART1_RX_BUF_SIZE;
		count++;
	}
	return count;
}

int UART1Driver_Write(unsigned char *buffer, unsigned int size)
{
	// Deactivate the irq so the data is all written in the buffer without problems.
	LPUART_DisableInterrupts(LPUART1,
			kLPUART_TxDataRegEmptyInterruptEnable);

	// Write all the data in the buffer.
	for (unsigned int i = 0; i < size; i++)
	{
		uart1TXBuffer[uart1TXIdx] = buffer[i];
		uart1TXIdx = (uart1TXIdx + 1) % UART1_TX_BUF_SIZE;
	}

	// Send the first byte.
	LPUART_WriteByte(LPUART1, uart1TXBuffer[uart1TXLastWrite]);
	uart1TXLastWrite = (uart1TXLastWrite + 1) % UART1_TX_BUF_SIZE;

	// Activate the irq.
	LPUART_EnableInterrupts(LPUART1,
			kLPUART_TxDataRegEmptyInterruptEnable);
	return size;
}

void LPUART1_IRQHandler()
{
	unsigned int flags = LPUART_GetStatusFlags(LPUART1);

	if (flags & kLPUART_RxDataRegFullFlag)
	{
		// Something was received by the uart.

		// Reset the interbyte timer.
		Serial_StartInterbyteTimer();
		// Save the received byte.
		uart1RXBuffer[uart1RXIdx] = LPUART_ReadByte(LPUART1);
		uart1RXIdx = (uart1RXIdx + 1) % UART1_RX_BUF_SIZE;
	}
	else if (flags & kLPUART_TxDataRegEmptyFlag)
	{
		// The uart is ready to send another byte.
		if (uart1TXIdx != uart1TXLastWrite)
		{
			// If the write buffer is not empty, send the next byte.
			LPUART_WriteByte(LPUART1, uart1TXBuffer[uart1TXLastWrite]);
			uart1TXLastWrite = (uart1TXLastWrite + 1) % UART1_TX_BUF_SIZE;
		}
		else
		{
			// Nothing more to send, deactivate the send irq.
			LPUART_DisableInterrupts(LPUART1,
					kLPUART_TxDataRegEmptyInterruptEnable);
		}
	}
}
