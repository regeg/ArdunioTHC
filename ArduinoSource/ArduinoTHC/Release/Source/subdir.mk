################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Source/Button.cpp \
../Source/BypassMode.cpp \
../Source/IO.cpp \
../Source/Messages.cpp \
../Source/NonVolatile.cpp \
../Source/StateCutting.cpp \
../Source/StateEnabled.cpp \
../Source/THC.cpp \
../Source/THCInterface.cpp \
../Source/Timer.cpp 

OBJS += \
./Source/Button.o \
./Source/BypassMode.o \
./Source/IO.o \
./Source/Messages.o \
./Source/NonVolatile.o \
./Source/StateCutting.o \
./Source/StateEnabled.o \
./Source/THC.o \
./Source/THCInterface.o \
./Source/Timer.o 

CPP_DEPS += \
./Source/Button.d \
./Source/BypassMode.d \
./Source/IO.d \
./Source/Messages.d \
./Source/NonVolatile.d \
./Source/StateCutting.d \
./Source/StateEnabled.d \
./Source/THC.d \
./Source/THCInterface.d \
./Source/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\arduino-1.0.2\hardware\arduino\cores\arduino" -I"C:\arduino-1.0.2\hardware\arduino\variants\mega" -I"C:\github-repo\ArduinoTHC\ArduinoSource\ArduinoTHC" -I"C:\arduino-1.0.2\libraries\EEPROM" -I"C:\arduino-1.0.2\libraries\LiquidCrystal" -D__IN_ECLIPSE__=1 -DUSB_VID= -DUSB_PID= -DARDUINO=102 -Wall -Os -ffunction-sections -fdata-sections -fno-exceptions -g -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"  -c -o "$@" -x c++ "$<"
	@echo 'Finished building: $<'
	@echo ' '


