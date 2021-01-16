################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sdmmc/host/fsl_sdmmc_host.c 

OBJS += \
./sdmmc/host/fsl_sdmmc_host.o 

C_DEPS += \
./sdmmc/host/fsl_sdmmc_host.d 


# Each subdirectory must supply rules for building sources it contributes
sdmmc/host/%.o: ../sdmmc/host/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1021DAG5A -DCPU_MIMXRT1021DAG5A_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING -DSD_ENABLED -DFSL_RTOS_FREE_RTOS -DSERIAL_PORT_TYPE_UART=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\board" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\source" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\drivers" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\device" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\CMSIS" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\sdmmc\inc" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\sdmmc\host" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\sdmmc\osa" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\component\osa" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\freertos\freertos_kernel\include" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\component\lists" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\utilities" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\component\serial_manager" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\component\uart" -I"D:\Fichier\DesinIV\MCU\Workspace\evkmimxrt1020_sdcard_freertos\xip" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

