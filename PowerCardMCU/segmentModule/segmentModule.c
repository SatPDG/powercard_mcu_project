/*
 * segmentModule.c
 *
 *  Created on: Feb. 16, 2021
 *      Author: Leo Clouet
 */

#include "segmentModule.h"

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_pit.h"

typedef struct
{
	GPIO_Type *gpioPort;
	unsigned int gpioPin;
	unsigned int muxRegister;
	unsigned int muxMode;
	unsigned int inputRegister;
	unsigned int inputDaisy;
	unsigned int configRegister;
} segmentGpio_t;

#define SEGMENT_SEG1 0
#define SEGMENT_SEG2 1
#define SEGMENT_SEG3 2
#define SEGMENT_SEG4 3
#define SEGMENT_SEG5 4
#define SEGMENT_SEG6 5
#define SEGMENT_SEG7 6
#define SEGMENT_SEG8 7
#define SEGMENT_POINT 8

const segmentGpio_t segmentList[9];
const segmentGpio_t digitList[3];

unsigned char segmentState[3][9];

void SegmentModule_Init()
{

}

void SegmentModule_Task()
{

}

void SegmentModule_UpdateDisplayValue(float val)
{

}
