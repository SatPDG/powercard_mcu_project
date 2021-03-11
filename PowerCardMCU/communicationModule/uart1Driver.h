/*
 * uart1Driver.h
 *
 *  Created on: Jan. 17, 2021
 *      Author: Leo Clouet
 */

#ifndef UART1DRIVER_H_
#define UART1DRIVER_H_

void UART1Driver_Init();

int UART1Driver_Read(unsigned char *buffer, unsigned int size);

int UART1Driver_Write(unsigned char *buffer, unsigned int size);

#endif /* UART1DRIVER_H_ */
