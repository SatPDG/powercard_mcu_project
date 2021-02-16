/*
 * crc16.h
 *
 *  Created on: Jan. 10, 2021
 *      Author: Leo Clouet
 */

#ifndef CRC16_H_
#define CRC16_H_

unsigned short CRC16_Compute(unsigned char *data, unsigned int size);

unsigned short CRC16_Accumulate(unsigned short crc, unsigned char *data, unsigned int size);

#endif /* CRC16_H_ */
