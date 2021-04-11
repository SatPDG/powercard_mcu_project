#include "adcDriver.h"

#include "fsl_clock.h"
#include "fsl_pit.h"
#include "fsl_adc.h"
#include "fsl_adc_etc.h"
#include "fsl_xbara.h"
#include "fsl_iomuxc.h"

#include "samplingModule.h"

#define PIT_FREQUENCY 24000000
#define SAMPLING_FREQUENCY 100

void ADCDriver_Init()
{

	// Init the timer that trigger tha sampling action.
	{
		CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
		CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);

		pit_config_t pitConfig;
		PIT_GetDefaultConfig(&pitConfig);
		PIT_Init(PIT, &pitConfig);
		PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, (PIT_FREQUENCY / SAMPLING_FREQUENCY) - 1);
	}

	// Init the link between the timer event and the adc_etc via the xbara.
	{
		CLOCK_EnableClock(kCLOCK_Xbar1);

		XBARA_Init(XBARA);
		XBARA_SetSignalsConnection(XBARA, kXBARA1_InputPitTrigger0,
				kXBARA1_OutputAdcEtcTrig00);
	}

	// Init the adc for sampling.
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

	// Init the adc_etc. This peripheral sample 3 channels one by one when an event occur.
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
		adcEtcTriggerChainConfig.ADCChannelSelect = 13;			// Battery current
		adcEtcTriggerChainConfig.InterruptEnable =
				kADC_ETC_InterruptDisable;
		ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 0U,
				&adcEtcTriggerChainConfig);

		adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 1;
		adcEtcTriggerChainConfig.ADCChannelSelect = 14;			// Battery voltage
		adcEtcTriggerChainConfig.InterruptEnable =
				kADC_ETC_InterruptDisable;
		ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 1U,
				&adcEtcTriggerChainConfig);

		adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U << 1;
		adcEtcTriggerChainConfig.ADCChannelSelect = 15;			// Temperature sensor
		adcEtcTriggerChainConfig.InterruptEnable =
				kADC_ETC_Done0InterruptEnable;
		ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 2U,
				&adcEtcTriggerChainConfig);

		// Enable the interrupt to the software can take the sample.
		NVIC_SetPriority(ADC_ETC_IRQ0_IRQn, 2);
		NVIC_EnableIRQ(ADC_ETC_IRQ0_IRQn);
	}

	// Init the adc pin
	{
		IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_13_GPIO1_IO29, 0xB0u);
		IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_14_GPIO1_IO30, 0xB0u);
		IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_15_GPIO1_IO31, 0xB0u);
	}
}

void ADCDriver_StartSampling()
{
	PIT_StartTimer(PIT, kPIT_Chnl_0);
}

void ADC_ETC_IRQ0_IRQHandler(void)
{
	ADC_ETC_ClearInterruptStatusFlags(ADC_ETC, kADC_ETC_Trg0TriggerSource,
			kADC_ETC_Done0StatusFlagMask);

	// Get the sample.
	unsigned short voltage = ADC_ETC_GetADCConversionValue(ADC_ETC, 0, 1);
	unsigned short current = ADC_ETC_GetADCConversionValue(ADC_ETC, 0, 0);
	unsigned short temperature = ADC_ETC_GetADCConversionValue(ADC_ETC, 0, 2);

	// Send the sample to the treatment.
	SamplingModule_PushSamples(voltage, current, temperature);

	__DSB();
}
