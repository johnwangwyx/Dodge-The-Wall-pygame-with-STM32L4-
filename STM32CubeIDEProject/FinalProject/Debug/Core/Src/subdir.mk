################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Src/AsmAtan2.s \
../Core/Src/arctan_lookup.s \
../Core/Src/my_atan2.s 

C_SRCS += \
../Core/Src/main.c \
../Core/Src/stm32l475e_iot01.c \
../Core/Src/stm32l475e_iot01_accelero.c \
../Core/Src/stm32l475e_iot01_qspi.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c 

OBJS += \
./Core/Src/AsmAtan2.o \
./Core/Src/arctan_lookup.o \
./Core/Src/main.o \
./Core/Src/my_atan2.o \
./Core/Src/stm32l475e_iot01.o \
./Core/Src/stm32l475e_iot01_accelero.o \
./Core/Src/stm32l475e_iot01_qspi.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o 

S_DEPS += \
./Core/Src/AsmAtan2.d \
./Core/Src/arctan_lookup.d \
./Core/Src/my_atan2.d 

C_DEPS += \
./Core/Src/main.d \
./Core/Src/stm32l475e_iot01.d \
./Core/Src/stm32l475e_iot01_accelero.d \
./Core/Src/stm32l475e_iot01_qspi.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.s Core/Src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I"/Users/johnwang/STM32CubeIDE/workspace_1.13.1/FinalProject/Drivers/Components" -I"/Users/johnwang/STM32CubeIDE/workspace_1.13.1/FinalProject/Drivers/Components/lsm6dsl" -I"/Users/johnwang/STM32CubeIDE/workspace_1.13.1/FinalProject/Core/Inc" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/AsmAtan2.d ./Core/Src/AsmAtan2.o ./Core/Src/arctan_lookup.d ./Core/Src/arctan_lookup.o ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/my_atan2.d ./Core/Src/my_atan2.o ./Core/Src/stm32l475e_iot01.cyclo ./Core/Src/stm32l475e_iot01.d ./Core/Src/stm32l475e_iot01.o ./Core/Src/stm32l475e_iot01.su ./Core/Src/stm32l475e_iot01_accelero.cyclo ./Core/Src/stm32l475e_iot01_accelero.d ./Core/Src/stm32l475e_iot01_accelero.o ./Core/Src/stm32l475e_iot01_accelero.su ./Core/Src/stm32l475e_iot01_qspi.cyclo ./Core/Src/stm32l475e_iot01_qspi.d ./Core/Src/stm32l475e_iot01_qspi.o ./Core/Src/stm32l475e_iot01_qspi.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su

.PHONY: clean-Core-2f-Src

