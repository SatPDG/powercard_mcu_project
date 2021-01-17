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

void Communication_Task() {
	com_queue = xQueueCreate(2, sizeof(serialRxPacket_t));
	serialRxPacket_t message;
	while (1) {
		if (xQueueReceive(com_queue, &message, portMAX_DELAY) == pdTRUE) {
			// Execute command
			comData_t responseData;
			responseData.size = 0;
			responseData.data = 0;
			unsigned int result = CommunicationExecutor_Execute(
					message.packet.numFunction, message.packet.numRegister,
					message.packet.data, message.packet.size - 2,
					&responseData);

			if (result) {
				comData_t outPacket;
				outPacket.data = 0;
				outPacket.size = 0;
				// Send ack packet
				CommunicationPacketBuilder_BuildAck(&message.packet,
						&responseData, &outPacket);
				xQueueSend(message.txQueue, &outPacket, 5);
			} else {
				// Send nack packet
				comData_t outPacket;
				outPacket.data = 0;
				outPacket.size = 0;
				// Send ack packet
				CommunicationPacketBuilder_BuildNack(&message.packet,
						&responseData, &outPacket);
				xQueueSend(message.txQueue, &outPacket, 5);
			}

			if(responseData.data){
				vPortFree(responseData.data);
			}
			if(message.packet.data){
				vPortFree(message.packet.data);
			}
		}
	}
}
