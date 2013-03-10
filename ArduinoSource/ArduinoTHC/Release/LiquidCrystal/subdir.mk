################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/arduino-1.0.2/libraries/LiquidCrystal/LiquidCrystal.cpp 

OBJS += \
./LiquidCrystal/LiquidCrystal.o 

CPP_DEPS += \
./LiquidCrystal/LiquidCrystal.d 


# Each subdirectory must supply rules for building sources it contributes
LiquidCrystal/LiquidCrystal.o: C:/arduino-1.0.2/libraries/LiquidCrystal/LiquidCrystal.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\arduino-1.0.2\hardware\arduino\cores\arduino" -I"C:\arduino-1.0.2\hardware\arduino\variants\mega" -I"C:\github-repo\ArduinoTHC\ArduinoSource\ArduinoTHC" -I"C:\arduino-1.0.2\libraries\EEPROM" -I"C:\arduino-1.0.2\libraries\LiquidCrystal" -D__IN_ECLIPSE__=1 -DUSB_VID= -DUSB_PID= -DARDUINO=102 -Wall -Os -ffunction-sections -fdata-sections -fno-exceptions -g -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"  -c -o "$@" -x c++ "$<"
	@echo 'Finished building: $<'
	@echo ' '


