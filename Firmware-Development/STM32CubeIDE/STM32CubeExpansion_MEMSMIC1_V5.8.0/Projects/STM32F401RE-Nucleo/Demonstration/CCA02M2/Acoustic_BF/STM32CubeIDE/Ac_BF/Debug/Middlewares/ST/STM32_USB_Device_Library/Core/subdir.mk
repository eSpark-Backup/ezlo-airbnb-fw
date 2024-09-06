################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Abeer.DESKTOP-Q8FRVD0/Downloads/en.x-cube-memsmic1/STM32CubeExpansion_MEMSMIC1_V5.8.0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
C:/Users/Abeer.DESKTOP-Q8FRVD0/Downloads/en.x-cube-memsmic1/STM32CubeExpansion_MEMSMIC1_V5.8.0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
C:/Users/Abeer.DESKTOP-Q8FRVD0/Downloads/en.x-cube-memsmic1/STM32CubeExpansion_MEMSMIC1_V5.8.0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c 

OBJS += \
./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_core.o \
./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ctlreq.o \
./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ioreq.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_core.d \
./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ctlreq.d \
./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Device_Library/Core/usbd_core.o: C:/Users/Abeer.DESKTOP-Q8FRVD0/Downloads/en.x-cube-memsmic1/STM32CubeExpansion_MEMSMIC1_V5.8.0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c Middlewares/ST/STM32_USB_Device_Library/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F446xx -D__FPU_PRESENT=1U -DUSE_STM32F4XX_NUCLEO -DUSE_USB_FS -DARM_MATH_CM4 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../../../../../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32F4xx-Nucleo -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../../../../../../Drivers/BSP/CCA02M2 -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../../../../../../Middlewares/ST/STM32_AcousticBF_Library/Inc -I"../../../../../../../../Middlewares/ST/STM32_AcousticBF_Library/Src" -O3 -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ctlreq.o: C:/Users/Abeer.DESKTOP-Q8FRVD0/Downloads/en.x-cube-memsmic1/STM32CubeExpansion_MEMSMIC1_V5.8.0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c Middlewares/ST/STM32_USB_Device_Library/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F446xx -D__FPU_PRESENT=1U -DUSE_STM32F4XX_NUCLEO -DUSE_USB_FS -DARM_MATH_CM4 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../../../../../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32F4xx-Nucleo -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../../../../../../Drivers/BSP/CCA02M2 -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../../../../../../Middlewares/ST/STM32_AcousticBF_Library/Inc -I"../../../../../../../../Middlewares/ST/STM32_AcousticBF_Library/Src" -O3 -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ioreq.o: C:/Users/Abeer.DESKTOP-Q8FRVD0/Downloads/en.x-cube-memsmic1/STM32CubeExpansion_MEMSMIC1_V5.8.0/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c Middlewares/ST/STM32_USB_Device_Library/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F446xx -D__FPU_PRESENT=1U -DUSE_STM32F4XX_NUCLEO -DUSE_USB_FS -DARM_MATH_CM4 -c -I../../../Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../../../../../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/STM32F4xx-Nucleo -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../../../../../../Drivers/BSP/CCA02M2 -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../../../../../../Middlewares/ST/STM32_AcousticBF_Library/Inc -I"../../../../../../../../Middlewares/ST/STM32_AcousticBF_Library/Src" -O3 -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Core

clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Core:
	-$(RM) ./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_core.cyclo ./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_core.d ./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_core.o ./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_core.su ./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ctlreq.cyclo ./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ctlreq.d ./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ctlreq.o ./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ctlreq.su ./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ioreq.cyclo ./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ioreq.d ./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ioreq.o ./Middlewares/ST/STM32_USB_Device_Library/Core/usbd_ioreq.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Core

