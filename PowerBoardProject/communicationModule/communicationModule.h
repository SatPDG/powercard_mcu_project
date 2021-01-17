/*
 * communicationModule.h
 *
 *  Created on: Jan. 10, 2021
 *      Author: Leo Clouet
 */

#ifndef COMMUNICATIONMODULE_H_
#define COMMUNICATIONMODULE_H_

#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t com_queue;

void Communication_Task();

#endif /* COMMUNICATIONMODULE_H_ */
