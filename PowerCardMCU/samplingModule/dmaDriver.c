/*
 * dmaDriver.c
 *
 *  Created on: Jan. 14, 2021
 *      Author: Leo Clouet
 */

#include "fsl_xbara.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"

void DMADriver_Init()
{
//	// Init xbara adc_etc -> dmamux
//	{
//		XBARA_SetSignalsConnection(XBARA, kXBARA1_InputAdcEtc0Coco0,
//				kXBARA1_OutputDmaChMuxReq30);
//		xbara_control_config_t control_config;
//		control_config.activeEdge = kXBARA_EdgeRising;
//		control_config.requestType = kXBARA_RequestDMAEnable;
//		XBARA_SetOutputSignalConfig(XBARA, kXBARA1_OutputDmaChMuxReq30, controlConfig);
//	}
//
//	// Init DMA mux
//	{
//		DMAMUX_Init(DMAMUX);
//		DMAMUX_SetSource(DMAMUX, 9, kDmaRequestMuxXBAR1Request0);
//		DMAMUX_EnableChannel(DMAMUX, 9);
//	}
}
