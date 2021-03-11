/*
 * uartTask.h
 *
 *  Created on: Mar. 11, 2021
 *      Author: Leo Clouet
 */

#ifndef SERIALTASK_H_
#define SERIALTASK_H_

#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t serialTX_queue;

void Serial_Init();

void Serial_StartInterbyteTimer();

void Serial_RXTask();

void Serial_TXTask();

#endif /* SERIALTASK_H_ */
