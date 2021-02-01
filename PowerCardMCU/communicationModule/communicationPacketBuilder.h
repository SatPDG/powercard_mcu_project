/*
 * CommunicationPacketBuilder.h
 *
 *  Created on: Jan. 10, 2021
 *      Author: Leo Clouet
 */

#ifndef COMMUNICATIONPACKETBUILDER_H_
#define COMMUNICATIONPACKETBUILDER_H_

#include "communicationProtocol.h"

void CommunicationPacketBuilder_BuildAck(comPacket_t *inPacket, comData_t *responseData, comData_t *outPacket);

void CommunicationPacketBuilder_BuildNack(comPacket_t *inPacket, comData_t *responseData, comData_t *outPacket);

#endif /* COMMUNICATIONPACKETBUILDER_H_ */
