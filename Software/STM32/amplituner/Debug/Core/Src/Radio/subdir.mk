################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Radio/RDA5807m.c 

OBJS += \
./Core/Src/Radio/RDA5807m.o 

C_DEPS += \
./Core/Src/Radio/RDA5807m.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Radio/%.o: ../Core/Src/Radio/%.c Core/Src/Radio/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4Q5xx -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -Wextra -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Radio

clean-Core-2f-Src-2f-Radio:
	-$(RM) ./Core/Src/Radio/RDA5807m.d ./Core/Src/Radio/RDA5807m.o

.PHONY: clean-Core-2f-Src-2f-Radio

