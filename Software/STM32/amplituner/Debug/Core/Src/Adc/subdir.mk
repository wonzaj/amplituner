################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Adc/UVmeter.c \
../Core/Src/Adc/arm_cfft_f32.c \
../Core/Src/Adc/arm_rfft_fast_f32.c \
../Core/Src/Adc/arm_rfft_fast_init_f32.c \
../Core/Src/Adc/fft.c 

S_UPPER_SRCS += \
../Core/Src/Adc/arm_bitreversal2.S 

OBJS += \
./Core/Src/Adc/UVmeter.o \
./Core/Src/Adc/arm_bitreversal2.o \
./Core/Src/Adc/arm_cfft_f32.o \
./Core/Src/Adc/arm_rfft_fast_f32.o \
./Core/Src/Adc/arm_rfft_fast_init_f32.o \
./Core/Src/Adc/fft.o 

S_UPPER_DEPS += \
./Core/Src/Adc/arm_bitreversal2.d 

C_DEPS += \
./Core/Src/Adc/UVmeter.d \
./Core/Src/Adc/arm_cfft_f32.d \
./Core/Src/Adc/arm_rfft_fast_f32.d \
./Core/Src/Adc/arm_rfft_fast_init_f32.d \
./Core/Src/Adc/fft.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Adc/%.o: ../Core/Src/Adc/%.c Core/Src/Adc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4Q5xx -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Adc/%.o: ../Core/Src/Adc/%.S Core/Src/Adc/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Src-2f-Adc

clean-Core-2f-Src-2f-Adc:
	-$(RM) ./Core/Src/Adc/UVmeter.d ./Core/Src/Adc/UVmeter.o ./Core/Src/Adc/arm_bitreversal2.d ./Core/Src/Adc/arm_bitreversal2.o ./Core/Src/Adc/arm_cfft_f32.d ./Core/Src/Adc/arm_cfft_f32.o ./Core/Src/Adc/arm_rfft_fast_f32.d ./Core/Src/Adc/arm_rfft_fast_f32.o ./Core/Src/Adc/arm_rfft_fast_init_f32.d ./Core/Src/Adc/arm_rfft_fast_init_f32.o ./Core/Src/Adc/fft.d ./Core/Src/Adc/fft.o

.PHONY: clean-Core-2f-Src-2f-Adc

