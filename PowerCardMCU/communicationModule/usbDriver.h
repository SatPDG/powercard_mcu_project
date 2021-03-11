/*
 * uart2Driver.h
 *
 *  Created on: Mar. 11, 2021
 *      Author: Leo Clouet
 */

#ifndef USBDRIVER_H_
#define USBDRIVER_H_

void USBDriver_Init();

int USBDriver_Read(unsigned char *buffer, unsigned int size);

int USBDriver_Write(unsigned char *buffer, unsigned int size);

#endif /* USBDRIVER_H_ */
