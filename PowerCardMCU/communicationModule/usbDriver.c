/*
 * usbDriver.c
 *
 *  Created on: Mar. 11, 2021
 *      Author: Leo Clouet
 */

#include "usbDriver.h"

#include "serialTask.h"
#include "fsl_lpuart.h"
#include "fsl_iomuxc.h"
#include "board.h"

#define USB_RX_BUF_SIZE 512
#define USB_TX_BUF_SIZE 512

// Driver attribute
unsigned char usbRXBuffer[USB_RX_BUF_SIZE];
unsigned char usbTXBuffer[USB_TX_BUF_SIZE];

unsigned int usbRXIdx = 0;
unsigned int usbRXLastRead = 0;
unsigned int usbTXIdx = 0;
unsigned int usbTXLastWrite = 0;

void USBDriver_Init()
{
	// Init uart pin
	{
		 IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_09_LPUART2_RX, 0U);
		 IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_08_LPUART2_TX, 0U);
		 IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_09_LPUART2_RX, 0x10B0u);
		 IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_08_LPUART2_TX, 0x10B0u);
	}
	// Init uart peripheral
	{
		lpuart_config_t config;

		LPUART_GetDefaultConfig(&config);
		config.baudRate_Bps = 115200;
		config.enableTx = true;
		config.enableRx = true;
		LPUART_Init(LPUART2, &config, BOARD_DebugConsoleSrcFreq());

		LPUART_EnableInterrupts(LPUART2, kLPUART_RxDataRegFullInterruptEnable);
		NVIC_SetPriority(LPUART2_IRQn, 2);
		EnableIRQ(LPUART2_IRQn);
	}
}

int USBDriver_Read(unsigned char *buffer, unsigned int size)
{
	unsigned int count = 0;
	while (usbRXLastRead != usbRXIdx)
	{
		buffer[count] = usbRXBuffer[usbRXLastRead];
		usbRXLastRead = (usbRXLastRead + 1) % USB_RX_BUF_SIZE;
		count++;
	}
	return count;
}

int USBDriver_Write(unsigned char *buffer, unsigned int size)
{
	LPUART_DisableInterrupts(LPUART2,
			kLPUART_TxDataRegEmptyInterruptEnable);

	for (unsigned int i = 0; i < size; i++)
	{
		usbTXBuffer[usbTXIdx] = buffer[i];
		usbTXIdx = (usbTXIdx + 1) % USB_TX_BUF_SIZE;
	}

	LPUART_WriteByte(LPUART2, usbTXBuffer[usbTXLastWrite]);
	usbTXLastWrite = (usbTXLastWrite + 1) % USB_TX_BUF_SIZE;

	LPUART_EnableInterrupts(LPUART2,
			kLPUART_TxDataRegEmptyInterruptEnable);
	return size;
}

void LPUART2_IRQHandler()
{
	unsigned int flags = LPUART_GetStatusFlags(LPUART2);

	if (flags & kLPUART_RxDataRegFullFlag)
	{
		Serial_StartInterbyteTimer();
		usbRXBuffer[usbRXIdx] = LPUART_ReadByte(LPUART2);
		usbRXIdx = (usbRXIdx + 1) % USB_RX_BUF_SIZE;

	}
	else if (flags & kLPUART_TxDataRegEmptyFlag)
	{
		if (usbTXIdx != usbTXLastWrite)
		{
			LPUART_WriteByte(LPUART2, usbTXBuffer[usbTXLastWrite]);
			usbTXLastWrite = (usbTXLastWrite + 1) % USB_TX_BUF_SIZE;
		}
		else
		{
			LPUART_DisableInterrupts(LPUART2,
					kLPUART_TxDataRegEmptyInterruptEnable);
		}
	}
}
