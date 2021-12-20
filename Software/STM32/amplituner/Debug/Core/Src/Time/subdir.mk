################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Time/time.c 

OBJS += \
./Core/Src/Time/time.o 

C_DEPS += \
./Core/Src/Time/time.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Time/%.o: ../Core/Src/Time/%.c Core/Src/Time/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4Q5xx -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Time

clean-Core-2f-Src-2f-Time:
	-$(RM) ./Core/Src/Time/time.d ./Core/Src/Time/time.o

.PHONY: clean-Core-2f-Src-2f-Time

