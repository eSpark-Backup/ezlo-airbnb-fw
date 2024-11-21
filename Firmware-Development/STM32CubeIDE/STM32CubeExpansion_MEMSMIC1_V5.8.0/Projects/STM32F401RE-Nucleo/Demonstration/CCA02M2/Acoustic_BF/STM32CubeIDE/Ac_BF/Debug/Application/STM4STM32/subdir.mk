################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
D:/GitHub/ezlo-airbnb-fw/Firmware-Development/STM32CubeIDE/STM32CubeExpansion_MEMSMIC1_V5.8.0/Projects/STM32F401RE-Nucleo/Demonstration/CCA02M2/Acoustic_BF/STM32CubeIDE/startup_stm32f401xe.s 

OBJS += \
./Application/STM4STM32/startup_stm32f401xe.o 

S_DEPS += \
./Application/STM4STM32/startup_stm32f401xe.d 


# Each subdirectory must supply rules for building sources it contributes
Application/STM4STM32/startup_stm32f401xe.o: D:/GitHub/ezlo-airbnb-fw/Firmware-Development/STM32CubeIDE/STM32CubeExpansion_MEMSMIC1_V5.8.0/Projects/STM32F401RE-Nucleo/Demonstration/CCA02M2/Acoustic_BF/STM32CubeIDE/startup_stm32f401xe.s Application/STM4STM32/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Application-2f-STM4STM32

clean-Application-2f-STM4STM32:
	-$(RM) ./Application/STM4STM32/startup_stm32f401xe.d ./Application/STM4STM32/startup_stm32f401xe.o

.PHONY: clean-Application-2f-STM4STM32

