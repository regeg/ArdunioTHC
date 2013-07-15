/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef THC_H_
#define THC_H_
#include "Arduino.h"

#include "Timer.h"
#include "Platform.h"

#define VERSION "0.f"

// Define the number of analog voltage readings to use to average
//#define NUM_READINGS 10
//
//#define NUM_DISPLAY_READINGS 20


// Define the noise floor for the analog voltage so you can
// display 0 when there is no voltage.
#define ANALOG_VOLTAGE_NOISE_FLOOR 0 // 170

// Define the minimum cut voltage to allow setting cruise control.
#define MIN_CRUISE_SET_VOLTAGE 0

//#define DEBUG 1
//#define DEBUG_TIMING 1 // Requires that "DEBUG" flag be set.



enum Command_e
{
	COMMAND_NONE,
	COMMAND_VOLT_UP,
	COMMAND_VOLT_DOWN,
	COMMAND_CALIBRATE_VOLTAGE,
	COMMAND_CRUISE_MENU,
	COMMAND_CRUISE_SET,
	COMMAND_RESET_USE_DATA,
	COMMAND_RESET_FLASH_BLOCK,
	COMMAND_SAVE_USAGE_DATA,
};

enum THCMode_e
{
	THC_MODE_DISABLED,
	THC_MODE_BYPASS,
	THC_MODE_CRUISE,
	THC_MODE_OPERATING
};

enum THCState_e
{
	THC_STATE_ENABLED,
	THC_STATE_CUTTING
};

enum THCUnit_e
{
	THC_UNIT_COUNTS = 1,
	THC_UNIT_VOLTS= 2
};


typedef struct
{
		unsigned char voltageScaleFactor;
		unsigned int pilotArcCount;
		unsigned int pierceCount;
		unsigned char tipHours;
		unsigned char tipMinutes;
		unsigned char tipSeconds;
		unsigned int voltSetPoint;
		THCState_e defaultState;
		unsigned long initialWriteCount;
		unsigned char flashBlock;
} THCStatData;

typedef struct
{
		THCState_e currentState;
		THCMode_e currentMode;
		THCUnit_e currentUnit;

		bool torchOnState;
		bool arcGoodState;
		bool arcStateChanged;
		unsigned int voltSetPoint;
		unsigned int currentVoltage;
		bool runVoltageControl;
		bool usageUpdated;
		bool torchUp;
		bool torchDown;

		Timer lastUseTimer;
		Timer torchOnTimer;
		Timer torchStabilizeTimer;
		Timer cutStartTime;

		bool kerfEncountered;
		unsigned int kerfVoltage;
		Timer kerfTimer;
} StateData;

extern HardwareSerial SERIAL;

extern volatile THCStatData usageData;
extern StateData currentStateData;

void AddSecondsToTipUse(THCStatData *usageData, unsigned int seconds);
//void sendVoltageMessage(unsigned int value, bool voltage);
void SendCutStartPacket();
void ChangeUnit();
void TestTorchOn(bool value);

#ifdef __cplusplus
extern "C"
	{
#endif

	void loop();
	void setup();

#ifdef __cplusplus
	} // extern "C"
#endif

//add your function definitions for the project THC here

//Do not add code below this line
#endif /* THC_H_ */

