################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Abeer.DESKTOP-Q8FRVD0/Downloads/en.x-cube-memsmic1/STM32CubeExpansion_MEMSMIC1_V5.8.0/Projects/STM32F401RE-Nucleo/Demonstration/CCA02M2/Microphones_Streaming/Patch/stm32f4xx_hal_pcd_patch.c 

OBJS += \
./Application/Patch/stm32f4xx_hal_pcd_patch.o 

C_DEPS += \
./Application/Patch/stm32f4xx_hal_pcd_patch.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Patch/stm32f4xx_hal_pcd_patch.o: C:/Users/Abeer.DESKTOP-Q8FRVD0/Downloads/en.x-cube-memsmic1/STM32CubeExpansion_MEMSMIC1_V5.8.0/Projects/STM32F401RE-Nucleo/Demonstration/CCA02M2/Microphones_Streaming/Patch/stm32f4xx_hal_pcd_patch.c Application/Patch/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F446xx -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -DUSE_USB_FS -DARM_MATH_CM4 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../../../../../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32F4xx-Nucleo -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../../../../../../Drivers/BSP/CCA02M2 -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -O3 -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-Application-2f-Patch

clean-Application-2f-Patch:
	-$(RM) ./Application/Patch/stm32f4xx_hal_pcd_patch.cyclo ./Application/Patch/stm32f4xx_hal_pcd_patch.d ./Application/Patch/stm32f4xx_hal_pcd_patch.o ./Application/Patch/stm32f4xx_hal_pcd_patch.su

.PHONY: clean-Application-2f-Patch

