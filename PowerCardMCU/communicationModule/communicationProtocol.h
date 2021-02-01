/*
 * communicationProtocol.h
 *
 *  Created on: Jan. 10, 2021
 *      Author: Leo Clouet
 */

#ifndef COMMUNICATIONPROTOCOL_H_
#define COMMUNICATIONPROTOCOL_H_

#include "FreeRTOS.h"
#include "queue.h"

#define COM_START_BYTE 0x77

typedef enum {
	comerr_regNotFound = 0x1, comerr_illegalAccess = 0x2, comerr_badFunction = 0x3, comerr_dataMissing = 0x4,
} commErrorCode;

typedef enum {
	cf_read = 0x1,
	cf_write = 0x2,
	cf_readList = 0x3,
	cf_writeList = 0x4,
	cf_ack = 0xB,
	cf_nack = 0xC,
} commFunctions;

typedef struct {
	unsigned char size;
	unsigned char numSeq;
	unsigned char numFunction;
	unsigned char numRegister;
	unsigned char *data;
} comPacket_t;

typedef struct {
	comPacket_t packet;
	QueueHandle_t txQueue;
} serialRxPacket_t;

typedef struct {
	unsigned int size;
	unsigned char *data;
} comData_t;

#endif /* COMMUNICATIONPROTOCOL_H_ */
