################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../xip/evkmimxrt1020_flexspi_nor_config.c \
../xip/fsl_flexspi_nor_boot.c 

OBJS += \
./xip/evkmimxrt1020_flexspi_nor_config.o \
./xip/fsl_flexspi_nor_boot.o 

C_DEPS += \
./xip/evkmimxrt1020_flexspi_nor_config.d \
./xip/fsl_flexspi_nor_boot.d 


# Each subdirectory must supply rules for building sources it contributes
xip/%.o: ../xip/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1021DAG5A -DCPU_MIMXRT1021DAG5A_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING -DSD_ENABLED -DFSL_RTOS_FREE_RTOS -DSERIAL_PORT_TYPE_UART=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\board" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\samplingModule" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\main" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\ledModule" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\drivers" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\device" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\CMSIS" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\component\osa" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\freertos\freertos_kernel\include" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\component\lists" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\utilities" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\component\serial_manager" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\component\uart" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\xip" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

