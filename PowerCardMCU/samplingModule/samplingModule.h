/*
 * samplingModule.h
 *
 *  Created on: Jan. 16, 2021
 *      Author: Leo Clouet
 */

#ifndef SAMPLINGMODULE_H_
#define SAMPLINGMODULE_H_

extern float voltage;
extern float current;
extern float power;
extern float temperature;

void SamplingModule_Init();

void SamplingModule_Task();

void SamplingModule_PushSamples(unsigned short voltage, unsigned short current, unsigned short temperature);

#endif /* SAMPLINGMODULE_H_ */
