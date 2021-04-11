/*
 * communicationModule.c
 *
 *  Created on: Jan. 10, 2021
 *      Author: Leo Clouet
 */

#include "communicationModule.h"
#include "communicationExecutor.h"
#include "communicationPacketBuilder.h"
#include "communicationProtocol.h"

#include <stdio.h>
#include <string.h>

QueueHandle_t com_queue;
unsigned char comBuffer[256];

void Communication_Task() {
	com_queue = xQueueCreate(2, sizeof(serialRxPacket_t));
	serialRxPacket_t message;

	while (1) {
		if (xQueueReceive(com_queue, &message, portMAX_DELAY) == pdTRUE) {
			// Create and set the response data.
			comData_t responseData;
			responseData.size = 0;
			responseData.data = comBuffer;

			// Reset the buffer to 0x00.
			memset(comBuffer, 0x0, 256);

			// Execute the command.
			unsigned int result = CommunicationExecutor_Execute(
					message.packet.numFunction, message.packet.numRegister,
					message.packet.data, message.packet.size - 4,
					&responseData);

			if (result) {
				// If the command execute well, send the ack response.
				comData_t outPacket;
				outPacket.data = 0;
				outPacket.size = 0;
				outPacket.driverFlags = message.driverFlags;
				// Send ack packet
				CommunicationPacketBuilder_BuildAck(&message.packet,
						&responseData, &outPacket);

				xQueueSend(message.txQueue, &outPacket, 0);
			} else {
				// If an error occur, send the nack response.
				comData_t outPacket;
				outPacket.data = 0;
				outPacket.size = 0;
				outPacket.driverFlags = message.driverFlags;
				// Send nack. packet
				CommunicationPacketBuilder_BuildNack(&message.packet,
						&responseData, &outPacket);
				xQueueSend(message.txQueue, &outPacket, 0);
			}

			// Free the data of the received message.
			if(message.packet.data){
				vPortFree(message.packet.data);
			}
		}
	}
}
