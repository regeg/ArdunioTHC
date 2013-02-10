/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

#include "THC.h"

#include "HardwareSerial.h"

#include "THCInterface.hpp"
#include "IOConfig.h"
#include "Timer.hpp"
#include "IO.hpp"
#include "NonVolatile.hpp"

#include "StateEnabled.hpp"
#include "StateCutting.hpp"
#include "BypassMode.hpp"


// Forward declaration for command processing function.
void processCommand(Command_e newCmd);

//
// Global Declarations
//
// Given the size and complexity of this application, its reasonable
// to just make the key variables global.
//

// UI Control Object (display and input buttons)
THCInterface display;

// Object used to maintain configuration information in flash.
NonVolatileStorage flash;

// Key global data structure.
StateData currentStateData;

// Tip usage parameters.
volatile THCStatData usageData;


//int currentVoltage = 0;
//volatile int intFlag = 0;

// Default object for the USB serial port.
extern HardwareSerial Serial;
// Default object for optional serial port for debug/PC control.
extern HardwareSerial Serial1;

bool sendRaw = true;
bool sendFiltered = false;
bool sendVolts = false;
bool sendCounts = true;


#define START_CUT_VOLTS_RAW		0x80
#define START_CUT_VOLTS_FLTRD	0x81
#define START_CUT_COUNT_RAW 	0x82
#define START_CUT_COUNT_FLTRD	0x83


void SendCutStartPacket()
	{
	if (sendRaw && sendVolts)
		Serial1.write(START_CUT_VOLTS_RAW);
	else if (sendRaw && sendCounts)
		Serial1.write(START_CUT_COUNT_RAW);
	else if (sendFiltered && sendVolts)
		Serial1.write(START_CUT_COUNT_FLTRD);
	else if (sendRaw && sendVolts)
		Serial1.write(START_CUT_COUNT_RAW);
	}


// Add time to the tip usage.
void AddSecondsToTipUse(THCStatData *usageData, unsigned int seconds)
	{
	unsigned int temp;

	temp = (unsigned int) usageData->tipSeconds + seconds;
	usageData->tipSeconds = temp % 60;
	// Calculate the number of minutes
	temp = temp / 60;
	temp += (unsigned int) usageData->tipMinutes;
	usageData->tipMinutes = temp % 60;
	usageData->tipHours += (temp / 60);

	}




//The setup function is called once at startup of the sketch
void setup()
	{
	Timer splashTime;

	// Start the first serial port for development diag output
	Serial.begin(115200);
	Serial.println("\nTHC Setup Complete");

	// Start the second serial port for runtime debug output.
	Serial1.begin(38400);
	Serial1.println(": Second serial port running\n");

	// Clear the display
	display.clear();
	// Put the splash screen up for a fix 2 seconds.
	splashTime.startTimer();
	// Display the splash screen.
	display.splash((char *) VERSION);

	// Initialize the state variables.
	currentStateData.torchOnState = false;
	currentStateData.arcGoodState = false;

	// Configure the I/O as the very first thing so
	// nothing gets turned on unexpectedly.
	setupIO();

	// Load all the data from the flash and set the default state.
	flash.DataLoad();
	flash.DataGetAll((THCStatData*) &usageData);
	//currentStateData.currentState = usageData.defaultState;
	currentStateData.currentMode = THC_MODE_DISABLED;
	currentStateData.currentState = THC_STATE_ENABLED;
	currentStateData.voltSetPoint = usageData.voltSetPoint;
	currentStateData.kerfEncountered = false;

/*
	for (currentStateData.index = 0; currentStateData.index < NUM_READINGS;
			currentStateData.index++)
		currentStateData.readings[currentStateData.index] = 0;
*/

	// Start the circular buffer of reads at 0.
	currentStateData.index = 0;
	currentStateData.displayIndex = 0;

		// If the splash display hasn't be up for 2 seconds, then wait.
	while (splashTime.elapsedMilliSeconds() < 2000)
		{
		}
	// Initialize the run time display
	display.initDisplay();

	display.Display_TargetVoltage(currentStateData.voltSetPoint);
	currentStateData.runVoltageControl = false;
	display.SetTorchState(TORCH_STATE_NONE);
	//display.setTHCState(currentStateData.currentState);

	//display.setMenu(MENU_OPERATING);
	display.TorchOn(currentStateData.torchOnState);
	display.ArcGood(currentStateData.arcGoodState);

	//display.setTHCState(currentStateData.currentState);
	}




void readVoltage(void)
	{
	int newVoltage; //, delta;
	unsigned int raw;

	// Get the new voltage.
	newVoltage = analogRead(IN_A_VOLTAGE);
	raw = (unsigned int) newVoltage;

	if (newVoltage <= ANALOG_VOLTAGE_NOISE_FLOOR)
		currentStateData.currentVoltage = 0;
	else
		currentStateData.currentVoltage = newVoltage;
/*
	if ((unsigned int) newVoltage > currentStateData.currentVoltage)
		delta = newVoltage - (int) currentStateData.currentVoltage;
	else
		delta = (int) currentStateData.currentVoltage - newVoltage;


	if (delta > 10)
		currentStateData.currentVoltage = newVoltage;
	else
		{
		// Now, calculate the new value by using 20% of the difference of the
		// old value and the new value.
		currentStateData.currentVoltage = currentStateData.currentVoltage +
				((newVoltage - (int) currentStateData.currentVoltage) / 5);
		}
*/

	if (currentStateData.torchOnState)
		{
		if (sendCounts)
			{
			if (sendRaw)
				sendVoltageMessage(raw, false);
			else if (sendFiltered)
				sendVoltageMessage(currentStateData.currentVoltage, false);
			}
		}

	}




void handleModeChange()
	{

	//
	// See if the user requested a mode change.
	if (display.modeChangedPressed() == true)
		{
		// Only allow mode change if there is no torch on or arc good signal
		// (this is a short-cut for checking to see if we're cutting.
		if (!currentStateData.torchOnState && !currentStateData.arcGoodState)
			{
			if (currentStateData.currentMode == THC_MODE_DISABLED)
				{
				DebugPrint("Mode Change to: THC_MODE_BYPASS\n");
				currentStateData.currentMode = THC_MODE_BYPASS;
				display.setMenu(MENU_BYPASS);
				}
			else if (currentStateData.currentMode  == THC_MODE_BYPASS)
				{
				DebugPrint("Mode Change to: THC_MODE_CRUISE\n");
				currentStateData.currentMode = THC_MODE_CRUISE;
				display.setMenu(MENU_CRUISE_MODE);
				}
			else if (currentStateData.currentMode  == THC_MODE_CRUISE)
				{
				DebugPrint("Mode Change to: THC_MODE_ENABLED\n");
				currentStateData.currentMode = THC_MODE_ENABLED;
				currentStateData.currentState = THC_STATE_ENABLED;
				display.setMenu(MENU_OPERATING);
				}
			else if (currentStateData.currentMode  == THC_MODE_ENABLED)
				{
				DebugPrint("Mode Change to: THC_MODE_DISABLED\n");
				currentStateData.currentMode = THC_MODE_DISABLED;
				display.setMenu(MENU_DISABLED);
				}
			}
		}
	}





// The loop function is called in an endless loop
void loop()
	{
	Command_e newCmd = COMMAND_NONE; // Initialization not necessary, but don't want the warning.
	unsigned int delayDisplayUpdate = 0;
	//unsigned long startTime;
	THCState_e oldState;
	// Define the button to switch modes.
	BypassMode *bypassHandler;
	unsigned long loopWaitTimer;

// unsigned long tempCtr = 0;


#ifdef DEBUG_TIMING
	Timer loopTimer;

	loopTimer.startTimer();
#endif

	bypassHandler = new BypassMode(&display);

	// Initialize the variable used to detect state changes.
	oldState = currentStateData.currentState;

	DebugPrint("THC Init Complete - starting main loop\n");

	currentStateData.currentMode = THC_MODE_ENABLED;

	// Set the menu based on the initial mode.
	if (currentStateData.currentMode == THC_MODE_BYPASS)
		display.setMenu(MENU_BYPASS);
	else if (currentStateData.currentMode == THC_MODE_CRUISE)
		display.setMenu(MENU_CRUISE_MODE);
	else if (currentStateData.currentMode  == THC_MODE_ENABLED)
		display.setMenu(MENU_OPERATING);
	else if (currentStateData.currentMode  == THC_MODE_DISABLED)
		display.setMenu(MENU_DISABLED);

	// Initialize the wait timer for the main loop.
	// This timer is used to ensure that you don't execute the control
	// loop any more often than every millisecond.
	loopWaitTimer = millis();

	//
	// Main control loop for THC.
	//
	while (1)
		{
		// Ensure a millisecond has elapsed before running again.
		// This should give a little more consistent behavior.
		while (loopWaitTimer == millis())
			{
			}
		// Reset the timer for the next loop.
		loopWaitTimer = millis();

#ifdef DEBUG_TIMING
		if (loopTimer.elapsedMilliSeconds() >= 1)
			DebugPrintLn(loopTimer.elapsedMilliSeconds());
		loopTimer.startTimer();
#endif

		// Read and filter the torch voltage.
		readVoltage();

		// Only update the display about 5 times a second (assuming a 1 ms loop time).
		if (++delayDisplayUpdate > 200)
			{
			// Use the current value to average for the display.
			display.Display_CurrentVoltage(currentStateData.currentVoltage/7);
			delayDisplayUpdate = 0;
			}

		// Check to see if the user has requested a mode change.
		// If so, it is handled within the routine.
		handleModeChange();

		//
		// Take the appropriate action based on the operational MODE.
		//

		// If in Disabled mode, don't do anything.
		if (currentStateData.currentMode == THC_MODE_DISABLED)
			{
			// Nothing is done when disabled.
			}
		// If in Bypass mode, used the handler that just passes through.
		else if (currentStateData.currentMode == THC_MODE_BYPASS)
			{
			bypassHandler->controlLoop();
			}
		// If in Cruise mode, used the handler that just passes through
		// until the user hits the "Select" button to start the THC function.
		else if (currentStateData.currentMode == THC_MODE_CRUISE)
			{
			bypassHandler->controlLoop();
			// Check for keypress to start controlling cutting.
			if (display.processCruiseButtons() == COMMAND_CRUISE_SET)
				{
				if ( currentStateData.torchOnState && currentStateData.arcGoodState &&
					 (currentStateData.currentVoltage > MIN_CRUISE_SET_VOLTAGE))
					{
					display.setMenu(MENU_OPERATING);
					currentStateData.voltSetPoint = currentStateData.currentVoltage;
					currentStateData.currentMode = THC_MODE_ENABLED;
					currentStateData.currentState = THC_STATE_CUTTING;
					currentStateData.runVoltageControl = true;
					}
				}
			}
		// In the Enabled mode, do the standard full height control processing.
		else if (currentStateData.currentMode == THC_MODE_ENABLED)
			{
			//
			// HANDLE STATE CHANGE
			//
			// Check to see if the State changed (cutting to enabled, or enabled to cutting).
			if (oldState != currentStateData.currentState)
				{
				// Update the display with the new state.
				display.setTHCState(currentStateData.currentState);
				DebugPrint("State Changed to: ");
				switch (currentStateData.currentState)
					{
					case THC_STATE_CUTTING:
						//DebugPrint("STATE CUTTING\n");
						//SendCutStartPacket();

#ifdef CAPTURE_VOLTAGE
						Serial1.print(
								currentStateData.cutStartTime.elapsedMilliSeconds());
						Serial1.print(": State Cutting - Set point - ");
						Serial1.println(currentStateData.voltSetPoint);
						startTime = millis();
#endif
						break;

					case THC_STATE_ENABLED:
						//DebugPrint("STATE ENABLED\n");
						// Update the torch state (up/down/okay) since we're cutting.
						display.SetTorchState(TORCH_STATE_NONE);
						break;
					}
				// Update the varIable used to track state changes.
				oldState = currentStateData.currentState;
				}

			//
			// RUN THE APPRORIATE HANDLER BASED ON THE STATE
			//

			// Run the state handler based on the current state,
			// either Enabled (waiting to cut) or cutting.
			if (currentStateData.currentState == THC_STATE_ENABLED)
				newCmd = StateEnabledHandler(&display);
			else if (currentStateData.currentState == THC_STATE_CUTTING)
				newCmd = StateCuttingHandler(&display);

			// Process any user commands via the buttons.
			processCommand(newCmd);
			}

		}  // While (1)

	}	// loop()


#ifdef AVERAGE_VOLTAGE
		// Subtract the reading to be overwritten from the sum.
		voltageSum -= currentStateData.readings[currentStateData.index];
		// Read another voltage sample into the buffer.
		currentStateData.readings[currentStateData.index] = analogRead(
				IN_A_VOLTAGE);

		if (currentStateData.readings[currentStateData.index] < 1)
		currentStateData.readings[currentStateData.index] = 0;

		// Add the new reading into the buffer.
		voltageSum += currentStateData.readings[currentStateData.index];
		currentStateData.currentVoltage = (unsigned int) (voltageSum
				/ (unsigned long) NUM_READINGS);
#endif

#ifdef DUMP_VOLTAGE
		// If we're cutting, output the new voltage and the average voltage.
		if (currentStateData.currentState == THC_STATE_CUTTING)
			{
			Serial1.print(currentStateData.cutStartTime.elapsedMilliSeconds());
			Serial1.print(": ");
			Serial1.println(currentStateData.currentVoltage);
			}
#endif

#ifdef AVERAGE_VOLTAGE
		// Increment the circular buffer pointer and see if it needs to wrap.
		if (++currentStateData.index >= NUM_READINGS)
			{
			// We've wrapped, so reset the pointer.
			currentStateData.index = 0;
			// Use the current value to average for the display.
			display.Display_CurrentVoltage(currentStateData.currentVoltage);
			}
#endif




void processCommand(Command_e newCmd)
	{
	// Process any command returned by the state handler.
	switch (newCmd)
		{
		default:
		case COMMAND_NONE:
			//Serial.print("COMMAND_NONE\n");
			break;

		case COMMAND_VOLT_UP:
			DebugPrint("COMMAND_VOLT_UP\n");
			if (currentStateData.voltSetPoint < 999)
				{
				usageData.voltSetPoint = ++currentStateData.voltSetPoint;
				// Start timer for delayed memory write
				currentStateData.lastUseTimer.startTimer();
				currentStateData.usageUpdated = true;
				}
			display.Display_TargetVoltage(currentStateData.voltSetPoint);
			break;

		case COMMAND_VOLT_DOWN:
			DebugPrint("COMMAND_VOLT_DOWN\n");
			if (currentStateData.voltSetPoint > 25)
				{
				usageData.voltSetPoint = --currentStateData.voltSetPoint;
				// Start timer for delayed memory write
				currentStateData.lastUseTimer.startTimer();
				currentStateData.usageUpdated = true;
				}
			display.Display_TargetVoltage(currentStateData.voltSetPoint);
			break;

		case COMMAND_CALIBRATE_VOLTAGE:
			DebugPrint("COMMAND_CALIBRATE_VOLTAGE\n");
			break;

		case COMMAND_RESET_USE_DATA:
			DebugPrint("COMMAND_RESET_USE_DATA\n");
			// Reset the usage data.
			flash.DataReset();
			// Reload the reset values.
			flash.DataGetAll((THCStatData*) &usageData);
			break;

		case COMMAND_RESET_FLASH_BLOCK:
			DebugPrint("COMMAND_RESET_FLASH_BLOCK\n");
			// Set the flash block to the next block after the current and
			// update the program variable with the block reference.
			usageData.flashBlock = flash.setBlock(flash.getBlock() + 1);
			// Flush all the usage data to the flash variables
			flash.DataSetAll((THCStatData*) &usageData);
			// Save the current usages to the new block.
			flash.DataSave();
			break;

		case COMMAND_SAVE_USAGE_DATA:
			// Copy the updated data to the flash object
			//usageData.voltSetPoint = currentStateData.voltSetPoint;
			flash.DataSetAll((THCStatData *) &usageData);
			// Write the object data to the memory.
			flash.DataSave();
			// Clear the new data flag.
			currentStateData.usageUpdated = false;
			display.setFlashWritePending(false);
			usageData.initialWriteCount++;
			break;

		}

	}

#ifdef AVERAGE_VOLTAGE
	unsigned long voltageSum, displayVoltageSum;
#endif
#ifdef AVERAGE_VOLTAGE
	// Initialize the voltage sum to 0.
	voltageSum = 0;
	displayVoltageSum = 0;
#endif

#ifdef AVERAGE_VOLTAGE
		// Subtract the reading to be overwritten from the sum.
		voltageSum -= currentStateData.readings[currentStateData.index];
		// Read another voltage sample into the buffer.
		currentStateData.readings[currentStateData.index] = analogRead(
				IN_A_VOLTAGE);

		if (currentStateData.readings[currentStateData.index] < 1)
		currentStateData.readings[currentStateData.index] = 0;

		// Add the new reading into the buffer.
		voltageSum += currentStateData.readings[currentStateData.index];
		currentStateData.currentVoltage = (unsigned int) (voltageSum
				/ (unsigned long) NUM_READINGS);
#endif


#define COUNT_MASK 			0x80
#define TORCH_ON_MASK 		0x40
#define ARC_GOOD_MASK 		0x20
#define VOLT_CTL_ON_MASK 	0x10
#define TORCH_UP_MASK		0x08
#define TORCH_DOWN_MASK		0x04
#define MSB_COUNTS_MASK 	0x03

void sendVoltageMessage(unsigned int value, bool voltage)
	{
	uint8_t byte[2];

	if (!voltage)
		{
		byte[0] = COUNT_MASK;
		byte[0] |= (char) ((value >> 8) & MSB_COUNTS_MASK);
		}
	else
		{
		byte[0] = 0;
		}

	byte[1] = (uint8_t) (value & 0xff);

	if (currentStateData.torchOnState)
		byte[0] |= TORCH_ON_MASK;

	if (currentStateData.arcGoodState)
		byte[0] |= ARC_GOOD_MASK;

	if (currentStateData.runVoltageControl)
		byte[0] |= VOLT_CTL_ON_MASK;

	if (currentStateData.torchUp)
		byte[0] |= TORCH_UP_MASK;

	if (currentStateData.torchDown)
		byte[0] |= TORCH_DOWN_MASK;

	Serial1.write((const uint8_t*) byte, 2);
	}


