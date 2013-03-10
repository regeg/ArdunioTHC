################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include arduino/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
libArduino_Mega_2560_or_Mega_ADK.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: libArduino_Mega_2560_or_Mega_ADK.a secondary-outputs

# Tool invocations
libArduino_Mega_2560_or_Mega_ADK.a: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR Archiver'
	avr-ar -r  "libArduino_Mega_2560_or_Mega_ADK.a" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

libArduino_Mega_2560_or_Mega_ADK.lss: libArduino_Mega_2560_or_Mega_ADK.a
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S libArduino_Mega_2560_or_Mega_ADK.a  >"libArduino_Mega_2560_or_Mega_ADK.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: libArduino_Mega_2560_or_Mega_ADK.a
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega2560 libArduino_Mega_2560_or_Mega_ADK.a
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(LSS)$(S_DEPS)$(CXX_DEPS)$(S_UPPER_DEPS)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(ARCHIVES)$(CPP_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) libArduino_Mega_2560_or_Mega_ADK.a
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
