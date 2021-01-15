################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_cache.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_usdhc.c 

OBJS += \
./drivers/fsl_cache.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_usdhc.o 

C_DEPS += \
./drivers/fsl_cache.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_usdhc.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MIMXRT1021DAG5A -DCPU_MIMXRT1021DAG5A_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 -DDEBUG_CONSOLE_TRANSFER_NON_BLOCKING -DSD_ENABLED -DFSL_RTOS_FREE_RTOS -DSERIAL_PORT_TYPE_UART=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\board" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\main" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\ledModule" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\drivers" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\device" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\CMSIS" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\component\osa" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\freertos\freertos_kernel\include" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\component\lists" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\utilities" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\component\serial_manager" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\component\uart" -I"D:\Fichier\DesignIV\MCU\powercard_mcu_project\PowerBoardProject\xip" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


