################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/LCD/LCD.c \
../Core/Inc/LCD/LCD_menus.c 

OBJS += \
./Core/Inc/LCD/LCD.o \
./Core/Inc/LCD/LCD_menus.o 

C_DEPS += \
./Core/Inc/LCD/LCD.d \
./Core/Inc/LCD/LCD_menus.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/LCD/%.o Core/Inc/LCD/%.su Core/Inc/LCD/%.cyclo: ../Core/Inc/LCD/%.c Core/Inc/LCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-LCD

clean-Core-2f-Inc-2f-LCD:
	-$(RM) ./Core/Inc/LCD/LCD.cyclo ./Core/Inc/LCD/LCD.d ./Core/Inc/LCD/LCD.o ./Core/Inc/LCD/LCD.su ./Core/Inc/LCD/LCD_menus.cyclo ./Core/Inc/LCD/LCD_menus.d ./Core/Inc/LCD/LCD_menus.o ./Core/Inc/LCD/LCD_menus.su

.PHONY: clean-Core-2f-Inc-2f-LCD

