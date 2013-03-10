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

#include "Messages.h"

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


// Flag value to determine when to send cutting voltages.
bool captureOn = false;


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
	Serial1.begin(115200);
	//Serial1.println("\nPort 1: THC Setup Complete");

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
	currentStateData.currentUnit = THC_UNIT_COUNTS;

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
		currentStateData.currentVoltage = raw;

	if ((currentStateData.torchOnState) && captureOn)
	{
		sendCutVoltageMessage();
	}

}




void handleModeChange()
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
			//Serial1.print(":");
			}
		// Reset the timer for the next loop.
		loopWaitTimer = millis();
		//Serial1.println(".");
#ifdef DEBUG_TIMING
		if (loopTimer.elapsedMilliSeconds() >= 1)
			DebugPrintLn(loopTimer.elapsedMilliSeconds());
		loopTimer.startTimer();
#endif

		// Read and filter the torch voltage.
		readVoltage();

		// Only update the display about 10 times a second (assuming a 1 ms loop time).
		if (++delayDisplayUpdate > 100)
			{
			// Use the current value for the display.
			/*
			if (currentStateData.currentUnit == THC_UNIT_VOLTS)
				display.Display_CurrentVoltage(currentStateData.currentVoltage/7);
			else
				display.Display_CurrentVoltage(currentStateData.currentVoltage);
				*/
			sendCutVoltageMessage();

			delayDisplayUpdate = 0;
			SendVoltage(PC_RESP_CURRENT);
			}

		// Check to see if the user has requested a mode change.
		// If so, it is handled within the routine.
		if (display.modeChangedPressed() == true)
			handleModeChange();

		if (Serial1.available() > 0)
		{
			uint8_t buff[10];
			uint16_t temp;

			switch (Serial1.peek())
			{
			case PC_CMD_SYNC:
				// Read and throw away the sync byte.
				Serial1.read();
				break;

			case PC_CMD_STEP_MODE:
				// Read and throw away the sync byte.
				Serial1.read();
				handleModeChange();
				// Build and send the response.
				SendMode();
				break;

			case PC_CMD_GET_MODE:
				// Read the command byte and throw away.
				Serial1.read();
				// Build and send the response.
				SendMode();
				break;

			case PC_CMD_STEP_UNIT:
				// Read the command byte and throw away.
				Serial1.read();
				// Update the unit.
				// Change the units between volts and counts.
				if (currentStateData.currentUnit == THC_UNIT_COUNTS)
					currentStateData.currentUnit = THC_UNIT_VOLTS;
				else
					currentStateData.currentUnit = THC_UNIT_COUNTS;

				// Build and send the responses.
				SendUnits();
				SendVoltage(PC_RESP_SETPOINT);
				SendVoltage(PC_RESP_CURRENT);
				display.ForceDisplay_CurrentVoltage(currentStateData.currentVoltage);
				display.Display_TargetVoltage(currentStateData.voltSetPoint);
				break;

			case PC_CMD_GET_UNIT:
				// Read the command byte and throw away.
				Serial1.read();
				SendUnits();
				break;

			case PC_CMD_DISPLAY_ON_OFF:
				if (Serial1.available() < 2)
					break;
				// Respond to the command.
				break;

			case PC_CMD_GET_STATUS:
				break;

			case PC_CMD_SET_SETPOINT:
				if (Serial1.available() < 3)
					break;
				// Read the command byte and throw away.
				Serial1.read();
				// Respond to the command.
				buff[0] = (uint8_t) Serial1.read();
				buff[1] = (uint8_t) Serial1.read();
				temp = (((uint16_t) buff[0]) << 8) | ((uint16_t) buff[1]);
				if (currentStateData.currentUnit == THC_UNIT_VOLTS)
					temp *= 7;
				currentStateData.voltSetPoint = temp;
				SendVoltage(PC_RESP_SETPOINT);
				break;

			case PC_CMD_GET_SETPOINT:
				// Read the command byte and throw away.
				Serial1.read();
				// Build and send the response.
				SendVoltage(PC_RESP_SETPOINT);
				break;

			case PC_CMD_GET_CURRENT:
				// Read the command byte and throw away.
				Serial1.read();
				// Build and send the response.
				SendVoltage(PC_RESP_CURRENT);
				break;

			case PC_CMD_TEST_TORCH_ON:
				// Read the command byte and throw away.
				Serial1.read();
				// Toggle the torch.
				TestTorchOn(true);
				break;

			case PC_CMD_TEST_TORCH_OFF:
				// Read the command byte and throw away.
				Serial1.read();
				// Toggle the torch.
				TestTorchOn(false);
				break;

			case PC_CMD_CAPTURE_START:
				// Read the command byte and throw away.
				Serial1.read();
				captureOn = true;
				sendVoltageCaptureState(captureOn);
				break;

			case PC_CMD_CAPTURE_STOP:
				// Read the command byte and throw away.
				Serial1.read();
				captureOn = false;
				sendVoltageCaptureState(captureOn);
				break;

			case PC_CMD_CAPTURE_GET_STATE:
				// Read the command byte and throw away.
				Serial1.read();
				sendVoltageCaptureState(captureOn);
				break;

			case PC_CMD_SETPOINT_UP:
				// Read the command byte and throw away.
				Serial1.read();
				// Handle the up.
				if ((currentStateData.currentUnit == THC_UNIT_VOLTS) && (currentStateData.voltSetPoint/7 < 180))
				{
					currentStateData.voltSetPoint += 7;
				}
				else if ((currentStateData.currentUnit == THC_UNIT_COUNTS) && (currentStateData.voltSetPoint < 1020))
				{
					currentStateData.voltSetPoint++;
				}
				// Build and send the response.
				SendVoltage(PC_RESP_SETPOINT);
				// Update the LCD
				display.Display_TargetVoltage(currentStateData.voltSetPoint);
				break;

			case PC_CMD_SETPOINT_DOWN:
				// Read the command byte and throw away.
				Serial1.read();
				// Handle the down.
				if ((currentStateData.currentUnit == THC_UNIT_VOLTS) && (currentStateData.voltSetPoint/7 > 50))
				{
					currentStateData.voltSetPoint -= 7;
				}
				else if ((currentStateData.currentUnit == THC_UNIT_COUNTS) && (currentStateData.voltSetPoint > 350))
				{
					currentStateData.voltSetPoint--;
				}
				// Build and send the response.
				SendVoltage(PC_RESP_SETPOINT);
				// Update the LCD
				display.Display_TargetVoltage(currentStateData.voltSetPoint);
				break;

			case PC_CMD_TEST_ARC_GOOD_ON:
				// Read the command byte and throw away.
				Serial1.read();
				// Turn on arc good for test
				digitalWrite(OUT_D_ARC_GOOD, OUT_D_ARC_GOOD_ACTIVE);
				break;

			case PC_CMD_TEST_ARC_GOOD_OFF:
				// Read the command byte and throw away.
				Serial1.read();
				// Turn off arc good for test
				digitalWrite(OUT_D_ARC_GOOD, OUT_D_ARC_GOOD_NOT_ACTIVE);
				break;

			case PC_CMD_TEST_THC_UP_ON:
				// Read the command byte and throw away.
				Serial1.read();
				// Turn on torch up for test
				digitalWrite(OUT_D_TORCH_UP, OUT_D_TORCH_UP_ACTIVE);
				break;

			case PC_CMD_TEST_THC_UP_OFF:
				// Read the command byte and throw away.
				Serial1.read();
				// Turn off torch up for test
				digitalWrite(OUT_D_TORCH_UP, OUT_D_TORCH_UP_NOT_ACTIVE);
				break;

			case PC_CMD_TEST_THC_DOWN_ON:
				// Read the command byte and throw away.
				Serial1.read();
				// Turn on torch down for test
				digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_ACTIVE);
				break;

			case PC_CMD_TEST_THC_DOWN_OFF:
				// Read the command byte and throw away.
				Serial1.read();
				// Turn off torch down for test
				digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_NOT_ACTIVE);
				break;



			default:
				break;
			}
		}
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
						SendCutStartPacket();

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




void TestTorchOn(bool value)
{
	if (value)
		// Turn on torch for test
		digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_ACTIVE);
	else
		// Turn off torch for test
		digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_NOT_ACTIVE);

}


