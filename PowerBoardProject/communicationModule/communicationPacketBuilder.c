
#include "communicationPacketBuilder.h"
#include "crc16.h"

void CommunicationPacketBuilder_BuildAck(comPacket_t *inPacket,
		comData_t *responseData, comData_t *outPacket) {
	unsigned int size = responseData->size + 4;
	if(inPacket->numFunction == cf_readList || inPacket->numFunction == cf_writeList){
		size += 2;
	}

	outPacket->data = pvPortMalloc(size + 2);
	outPacket->size = size + 2;

	unsigned char *packet = outPacket->data;
	packet[0] = COM_START_BYTE;
	packet[1] = size;
	packet[2] = (inPacket->numSeq << 4) | cf_ack;
	packet[3] = inPacket->numRegister;

	unsigned int offset = 0;
	if(inPacket->numFunction == cf_readList || inPacket->numFunction == cf_writeList){
		packet[4] = inPacket->data[0];
		packet[5] = inPacket->data[1];
		offset = 3;
	}

	for(unsigned int i = 0; i < responseData->size; i++){
		packet[3 + offset + i] = responseData->data[i];
	}
	offset += responseData->size;

	unsigned short crc = CRC16_Compute(packet, size);
	packet[3 + offset] = ((crc >> 8) & 0xFF);
	packet[4 + offset] = (crc & 0xFF);
}

void CommunicationPacketBuilder_BuildNack(comPacket_t *inPacket,
		comData_t *responseData, comData_t *outPacket) {
	unsigned int size = responseData->size + 4;
		if(inPacket->numFunction == cf_readList || inPacket->numFunction == cf_writeList){
			size += 2;
		}

		outPacket->data = pvPortMalloc(size + 2);
		outPacket->size = size + 2;

		unsigned char *packet = outPacket->data;
		packet[0] = COM_START_BYTE;
		packet[1] = size;
		packet[2] = (inPacket->numSeq << 4) | cf_nack;
		packet[3] = inPacket->numRegister;

		unsigned int offset = 0;
		if(inPacket->numFunction == cf_readList || inPacket->numFunction == cf_writeList){
			packet[4] = inPacket->data[0];
			packet[5] = inPacket->data[1];
			offset = 3;
		}

		for(unsigned int i = 0; i < responseData->size; i++){
			packet[3 + offset + i] = responseData->data[i];
		}
		offset += responseData->size;

		unsigned short crc = CRC16_Compute(packet, size);
		packet[3 + offset] = ((crc >> 8) & 0xFF);
		packet[4 + offset] = (crc & 0xFF);
}
