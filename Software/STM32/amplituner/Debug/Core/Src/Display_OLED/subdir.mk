################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Display_OLED/SSD1322_API.c \
../Core/Src/Display_OLED/SSD1322_GFX.c \
../Core/Src/Display_OLED/SSD1322_HW_Driver.c \
../Core/Src/Display_OLED/draws_display.c 

OBJS += \
./Core/Src/Display_OLED/SSD1322_API.o \
./Core/Src/Display_OLED/SSD1322_GFX.o \
./Core/Src/Display_OLED/SSD1322_HW_Driver.o \
./Core/Src/Display_OLED/draws_display.o 

C_DEPS += \
./Core/Src/Display_OLED/SSD1322_API.d \
./Core/Src/Display_OLED/SSD1322_GFX.d \
./Core/Src/Display_OLED/SSD1322_HW_Driver.d \
./Core/Src/Display_OLED/draws_display.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Display_OLED/%.o: ../Core/Src/Display_OLED/%.c Core/Src/Display_OLED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4Q5xx -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -Wextra -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Display_OLED

clean-Core-2f-Src-2f-Display_OLED:
	-$(RM) ./Core/Src/Display_OLED/SSD1322_API.d ./Core/Src/Display_OLED/SSD1322_API.o ./Core/Src/Display_OLED/SSD1322_GFX.d ./Core/Src/Display_OLED/SSD1322_GFX.o ./Core/Src/Display_OLED/SSD1322_HW_Driver.d ./Core/Src/Display_OLED/SSD1322_HW_Driver.o ./Core/Src/Display_OLED/draws_display.d ./Core/Src/Display_OLED/draws_display.o

.PHONY: clean-Core-2f-Src-2f-Display_OLED

