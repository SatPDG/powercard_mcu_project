#include "adcDriver.h"

#include "fsl_clock.h"
#include "fsl_pit.h"
#include "fsl_adc.h"
#include "fsl_adc_etc.h"
#include "fsl_xbara.h"
#include "fsl_iomuxc.h"

#include "samplingModule.h"

void ADCDriver_Init()
{

	// Init timer
	{
		CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
		CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);

		pit_config_t pitConfig;
		PIT_GetDefaultConfig(&pitConfig);
		PIT_Init(PIT, &pitConfig);
		PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, (24000000 / 100) - 1);
		//PIT_EnableInterrupts(PIT, 0, kPIT_TimerInterruptEnable);

		//NVIC_EnableIRQ(PIT_IRQn);
	}

	// Init xbara pit -> adc etc
	{
		CLOCK_EnableClock(kCLOCK_Xbar1);

		XBARA_Init(XBARA);
		XBARA_SetSignalsConnection(XBARA, kXBARA1_InputPitTrigger0,
				kXBARA1_OutputAdcEtcTrig00);
	}

	// Init adc
	{
		adc_config_t k_adcConfig;
		adc_channel_config_t adcChannelConfigStruct;

		/* Initialize the ADC module. */
		ADC_GetDefaultConfig(&k_adcConfig);
		k_adcConfig.resolution = kADC_Resolution12Bit;
		ADC_Init(ADC1, &k_adcConfig);
		ADC_EnableHardwareTrigger(ADC1, true);

		adcChannelConfigStruct.channelNumber = 16U;
		adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;
		ADC_SetChannelConfig(ADC1, 0, &adcChannelConfigStruct);
		ADC_SetChannelConfig(ADC1, 1, &adcChannelConfigStruct);

		ADC_DoAutoCalibration(ADC1);
	}

	// Init adc_etc
	{
		adc_etc_config_t adcEtcConfig;
		adc_etc_trigger_config_t adcEtcTriggerConfig;
		adc_etc_trigger_chain_config_t adcEtcTriggerChainConfig;

		ADC_ETC_GetDefaultConfig(&adcEtcConfig);
		adcEtcConfig.XBARtriggerMask = 1U; /* Enable the external XBAR trigger0. */
		ADC_ETC_Init(ADC_ETC, &adcEtcConfig);

		adcEtcTriggerConfig.enableSyncMode = false;
		adcEtcTriggerConfig.enableSWTriggerMode = false;
		adcEtcTriggerConfig.triggerChainLength = (3) - 1; /* Chain length is 2. */
		adcEtcTriggerConfig.triggerPriority = 0U;
		adcEtcTriggerConfig.sampleIntervalDelay = 0U;
		adcEtcTriggerConfig.initialDelay = 0U;
		ADC_ETC_SetTriggerConfig(ADC_ETC, 0U, &adcEtcTriggerConfig);

		adcEtcTriggerChainConfig.enableB2BMode = true;
		adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 0;
		adcEtcTriggerChainConfig.ADCChannelSelect = 10;
		adcEtcTriggerChainConfig.InterruptEnable =
				kADC_ETC_InterruptDisable;
		ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 0U,
				&adcEtcTriggerChainConfig);

		adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 1;
		adcEtcTriggerChainConfig.ADCChannelSelect = 11;
		adcEtcTriggerChainConfig.InterruptEnable =
				kADC_ETC_InterruptDisable;
		ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 1U,
				&adcEtcTriggerChainConfig);

		adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 1;
		adcEtcTriggerChainConfig.ADCChannelSelect = 12;
		adcEtcTriggerChainConfig.InterruptEnable =
				kADC_ETC_Done0InterruptEnable;
		ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 2U,
				&adcEtcTriggerChainConfig);

		NVIC_SetPriority(ADC_ETC_IRQ0_IRQn, 2);
		NVIC_EnableIRQ(ADC_ETC_IRQ0_IRQn);
		//NVIC_EnableIRQ(ADC_ETC_IRQ1_IRQn);
	}

	// Pin init
	{
		IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_10_GPIO1_IO26, 0xB0u);
		IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_11_GPIO1_IO27, 0xB0u);
		IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_12_GPIO1_IO28, 0xB0u);
		IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_13_GPIO1_IO29, 0xB0u);
		IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_14_GPIO1_IO30, 0xB0u);
		IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_15_GPIO1_IO31, 0xB0u);
	}
}

void ADCDriver_StartSampling()
{
	PIT_StartTimer(PIT, kPIT_Chnl_0);
}

//void PIT_IRQHandler()
//{
//	PIT_ClearStatusFlags(PIT, 0, kPIT_TimerFlag);
//}

void ADC_ETC_IRQ0_IRQHandler(void)
{
	ADC_ETC_ClearInterruptStatusFlags(ADC_ETC, kADC_ETC_Trg0TriggerSource,
			kADC_ETC_Done0StatusFlagMask);

	unsigned short voltage = ADC_ETC_GetADCConversionValue(ADC_ETC, 0, 0);
	unsigned short current = ADC_ETC_GetADCConversionValue(ADC_ETC, 0, 0);
	unsigned short temperature = ADC_ETC_GetADCConversionValue(ADC_ETC, 0, 0);

	SamplingModule_PushSamples(voltage, current, temperature);

	__DSB();
}
