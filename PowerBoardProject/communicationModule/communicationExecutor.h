/*
 * communicationExecutor.h
 *
 *  Created on: Jan. 10, 2021
 *      Author: Leo Clouet
 */

#ifndef COMMUNICATIONEXECUTOR_H_
#define COMMUNICATIONEXECUTOR_H_

#include "communicationProtocol.h"

unsigned int CommunicationExecutor_Execute(unsigned char function, unsigned char registerNo, unsigned char *data, unsigned int size, comData_t *responseData);

#endif /* COMMUNICATIONEXECUTOR_H_ */
