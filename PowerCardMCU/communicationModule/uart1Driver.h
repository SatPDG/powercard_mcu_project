/*
 * uart1Driver.h
 *
 *  Created on: Jan. 17, 2021
 *      Author: Leo Clouet
 */

#ifndef UART1DRIVER_H_
#define UART1DRIVER_H_

#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t uart1TX_queue;

void UART1Driver_Init();

void UART1Driver_RXTask();

void UART1Driver_TXTask();

#endif /* UART1DRIVER_H_ */
