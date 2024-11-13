################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/Audio-in/subdir.mk
-include Middlewares/ST/STM32_Acoustic_BF_Library/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Drivers/BSP/X-NUCLEO-CCA02M2/subdir.mk
-include Drivers/BSP/STM32F4xx_Nucleo/subdir.mk
-include Application/User/subdir.mk
-include Application/STM4STM32/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Ac_BF
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Ac_BF.elf \

MAP_FILES += \
Ac_BF.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Ac_BF.list \


# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: Ac_BF.elf secondary-outputs

# Tool invocations
Ac_BF.elf Ac_BF.map: $(OBJS) $(USER_OBJS) C:\Users\Ash\Downloads\x-cube-memsmic1-main\Projects\STM32F401RE-Nucleo\Demonstration\CCA02M2\Acoustic_BF\STM32CubeIDE\Ac_BF\STM32F401RETx_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Ac_BF.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Ash\Downloads\x-cube-memsmic1-main\Projects\STM32F401RE-Nucleo\Demonstration\CCA02M2\Acoustic_BF\STM32CubeIDE\Ac_BF\STM32F401RETx_FLASH.ld" -Wl,-Map="Ac_BF.map" -Wl,--gc-sections -static -L../../../../../../../../Middlewares/ST/STM32_Audio/Addons/PDM/Lib/ -L../../../../../../../../Drivers/CMSIS/DSP/Lib/GCC  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Ac_BF.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Ac_BF.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Ac_BF.elf Ac_BF.list Ac_BF.map default.size.stdout
	-@echo ' '

post-build:
	arm-none-eabi-objcopy -O binary "Ac_BF.elf" "Ac_BF.bin"
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified post-build

-include ../makefile.targets
