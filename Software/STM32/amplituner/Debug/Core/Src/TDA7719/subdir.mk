################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/TDA7719/TDA7719.c 

OBJS += \
./Core/Src/TDA7719/TDA7719.o 

C_DEPS += \
./Core/Src/TDA7719/TDA7719.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/TDA7719/%.o: ../Core/Src/TDA7719/%.c Core/Src/TDA7719/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4Q5xx -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -Wextra -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-TDA7719

clean-Core-2f-Src-2f-TDA7719:
	-$(RM) ./Core/Src/TDA7719/TDA7719.d ./Core/Src/TDA7719/TDA7719.o

.PHONY: clean-Core-2f-Src-2f-TDA7719

