
#include "communicationPacketBuilder.h"
#include "crc16.h"

void CommunicationPacketBuilder_BuildAck(comPacket_t *inPacket,
		comData_t *responseData, comData_t *outPacket) {

	// Compute the total response packet size and allocate the data.
	unsigned int size = responseData->size + 4;
	if(inPacket->numFunction == cf_readList || inPacket->numFunction == cf_writeList){
		size += 2;
	}
	outPacket->size = size + 2;
	outPacket->data = pvPortMalloc(outPacket->size);

	// Set the packet field.
	unsigned char *packet = outPacket->data;
	packet[0] = COM_START_BYTE;
	packet[1] = size;
	packet[2] = (inPacket->numSeq << 4) | cf_ack;
	packet[3] = inPacket->numRegister;

	unsigned int offset = 0;
	if(inPacket->numFunction == cf_readList || inPacket->numFunction == cf_writeList){
		packet[4] = inPacket->data[0];
		packet[5] = inPacket->data[1];
		offset = 2;
	}

	// Write the data in the packet.
	for(unsigned int i = 0; i < responseData->size; i++){
		packet[4 + offset + i] = responseData->data[i];
	}
	offset += responseData->size;

	// Compute and add the crc16.
	unsigned short crc = CRC16_Compute(packet, size);
	packet[4 + offset] = ((crc >> 8) & 0xFF);
	packet[5 + offset] = (crc & 0xFF);
}

void CommunicationPacketBuilder_BuildNack(comPacket_t *inPacket,
		comData_t *responseData, comData_t *outPacket) {

	// Compute the total response packet size and allocate the data.
	unsigned int size = responseData->size + 4;
	if(inPacket->numFunction == cf_readList || inPacket->numFunction == cf_writeList){
		size += 2;
	}
	outPacket->data = pvPortMalloc(size + 2);
	outPacket->size = size + 2;

	// Set the packet field.
	unsigned char *packet = outPacket->data;
	packet[0] = COM_START_BYTE;
	packet[1] = size;
	packet[2] = (inPacket->numSeq << 4) | cf_nack;
	packet[3] = inPacket->numRegister;

	unsigned int offset = 0;
	if(inPacket->numFunction == cf_readList || inPacket->numFunction == cf_writeList){
		packet[4] = inPacket->data[0];
		packet[5] = inPacket->data[1];
		offset = 2;
	}

	// Write the data in the packet.
	for(unsigned int i = 0; i < responseData->size; i++){
		packet[4 + offset + i] = responseData->data[i];
	}
	offset += responseData->size;

	// Compute and add the crc16.
	unsigned short crc = CRC16_Compute(packet, size);
	packet[4 + offset] = ((crc >> 8) & 0xFF);
	packet[5 + offset] = (crc & 0xFF);
}
