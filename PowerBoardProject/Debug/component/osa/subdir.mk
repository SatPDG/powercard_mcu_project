################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/osa/fsl_os_abstraction_free_rtos.c 

OBJS += \
./component/osa/fsl_os_abstraction_free_rtos.o 

C_DEPS += \
./component/osa/fsl_os_abstraction_free_rtos.d 


# Each subdirectory must supply rules for building sources it contributes
component/osa/%.o: ../component/osa/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DSERIAL_PORT_TYPE_UART=2 -DCPU_MIMXRT1021DAG5A -DCPU_MIMXRT1021DAG5A_cm7 -DSDK_DEBUGCONSOLE=1 -DXIP_EXTERNAL_FLASH=1 -DXIP_BOOT_HEADER_ENABLE=1 -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 -DSD_ENABLED -DFSL_RTOS_FREE_RTOS -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\board" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\util" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\communicationModule" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\samplingModule" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\main" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\ledModule" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\drivers" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\device" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\CMSIS" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\component\osa" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\freertos\freertos_kernel\include" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\component\lists" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\utilities" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\component\serial_manager" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\component\uart" -I"C:\Users\RMDS\Desktop\Design4\powercard_mcu_project\PowerBoardProject\xip" -O0 -fno-common -g3 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


