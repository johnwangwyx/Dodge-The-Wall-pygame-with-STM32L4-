################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Components/cy8c4014lqi/cy8c4014lqi.c 

OBJS += \
./Drivers/Components/cy8c4014lqi/cy8c4014lqi.o 

C_DEPS += \
./Drivers/Components/cy8c4014lqi/cy8c4014lqi.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Components/cy8c4014lqi/%.o Drivers/Components/cy8c4014lqi/%.su Drivers/Components/cy8c4014lqi/%.cyclo: ../Drivers/Components/cy8c4014lqi/%.c Drivers/Components/cy8c4014lqi/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I"/Users/johnwang/STM32CubeIDE/workspace_1.13.1/FinalProject/Drivers/Components" -I"/Users/johnwang/STM32CubeIDE/workspace_1.13.1/FinalProject/Drivers/Components/lsm6dsl" -I"/Users/johnwang/STM32CubeIDE/workspace_1.13.1/FinalProject/Core/Inc" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Components-2f-cy8c4014lqi

clean-Drivers-2f-Components-2f-cy8c4014lqi:
	-$(RM) ./Drivers/Components/cy8c4014lqi/cy8c4014lqi.cyclo ./Drivers/Components/cy8c4014lqi/cy8c4014lqi.d ./Drivers/Components/cy8c4014lqi/cy8c4014lqi.o ./Drivers/Components/cy8c4014lqi/cy8c4014lqi.su

.PHONY: clean-Drivers-2f-Components-2f-cy8c4014lqi

