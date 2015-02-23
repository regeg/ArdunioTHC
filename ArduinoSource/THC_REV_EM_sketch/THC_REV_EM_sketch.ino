/*
 * COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
 */
#include <EEPROM.h>
#include "Arduino.h"

#include "THC.h"

#include "Platform.h"

#include "HardwareSerial.h"

#include "IOConfig.h"
#include "Timer.h"
#include "IO.h"
#include "NonVolatile.h"

#include "StateEnabled.h"
#include "StateCutting.h"
#include "BypassMode.h"
#include "IO.h"
#include "Messages.h"

//
// Global Declarations
//
// Given the size and complexity of this application, its reasonable
// to just make the key variables global.
//

// Object used to maintain configuration information in flash.
NonVolatileStorage flash;

// Key global data structure.
StateData currentStateData;

// Tip usage parameters.
volatile THCStatData usageData;

// Flag value to determine when to send cutting voltages.
bool captureOn = true;

unsigned int displayUpdateDelay = 0;




//
// AddSecondsToTipUse()
//
// The amount of time cutting is maintained in the "usageData"
// structure so consumables life can be tracked.  When adding
// cutting time you add seconds, but it can effect minutes
// and hours.  So do the math properly.
//
void AddSecondsToTipUse(THCStatData *usageData, unsigned int seconds)
{
	unsigned int temp;

	temp = (unsigned int) usageData->tipSeconds + seconds;
	usageData->tipSeconds = temp % 60;
	// Calculate the number of minutes rolled over
	temp = (temp / 60) + ((unsigned int) usageData->tipMinutes);
	usageData->tipMinutes = temp % 60;
	usageData->tipHours += (temp / 60);
}




//
// setup()
//
// Initialization routine that will get the THC up and running.
// It starts the serial port, does the call to setup all the
// inputs and outputs and loads saved settings from flash.
//
void setup()
{

	// Start the first serial port for development diag output
	SERIAL.begin(115200);

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
	currentStateData.currentMode = THC_MODE_OPERATING;
	currentStateData.currentState = THC_STATE_ENABLED;
	currentStateData.voltSetPoint = usageData.voltSetPoint;
	currentStateData.kerfEncountered = false;
	currentStateData.currentUnit = THC_UNIT_COUNTS;
// TEMP SET VOLTAGE
	currentStateData.voltSetPoint = 760;
}




//
// handleModeChange()
//
// This routine will cause the controller to step through the
// different operating modes sequentially, as long as the
// TorchOn or ArcGood are not active.
//
// The sequence that modes are stepped through is:
// - Disabled (doesn't pass through torch on/off commands)
// - Bypass (passes through commands so you can cut w/o THC control)
// - Cruise (passes through until you tell it to use the current voltage
//           as the set point and start THC control)
// - Enabled (normal THC control)
//
void handleModeChange()
{

	// Only allow mode change if there is no torch on or arc good signal
	// (this is a short-cut for checking to see if we're cutting.
	if (!currentStateData.torchOnState && !currentStateData.arcGoodState)
	{
		if (currentStateData.currentMode == THC_MODE_DISABLED)
		{
			currentStateData.currentMode = THC_MODE_BYPASS;
		}
		else if (currentStateData.currentMode == THC_MODE_BYPASS)
		{
			currentStateData.currentMode = THC_MODE_CRUISE;
		}
		else if (currentStateData.currentMode == THC_MODE_CRUISE)
		{
  			currentStateData.currentMode = THC_MODE_OPERATING;
			currentStateData.currentState = THC_STATE_ENABLED;
		}
		else if (currentStateData.currentMode == THC_MODE_OPERATING)
		{
			currentStateData.currentMode = THC_MODE_DISABLED;
		}
	}
}




// The loop function is called in an endless loop
void loop()
{
	bool oldTorchState = true;
	BypassMode *bypassHandler;
	unsigned long loopWaitTimer;
	unsigned int newVoltage;

	bypassHandler = new BypassMode();

	// Initialize the wait timer for the main loop.
	// This timer is used to ensure that you don't execute the control
	// loop any more often than every millisecond.
	loopWaitTimer = millis();

        oldTorchState = currentStateData.torchOnState = false;

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

		//
		// READ THE TORCH VOLTAGE FROM THE PLASMA
		//
		newVoltage = analogRead(IN_A_VOLTAGE);
		// Filter any noise when off to 0.
		if (newVoltage <= ANALOG_VOLTAGE_NOISE_FLOOR)
			currentStateData.currentVoltage = 0;
		else
			currentStateData.currentVoltage = newVoltage;

		//
		// READ TORCH ON INPUT SIGNAL FROM CNC
		//
		currentStateData.torchOnState = (
				digitalRead(IN_D_TORCH_ON_PIN) == IN_D_TORCH_ON_PIN_ACTIVE ? true : false);

		// If the torch on state has changed, handle it.
		if (currentStateData.torchOnState != oldTorchState)
		{
			// Update saved torch on state.
			oldTorchState = currentStateData.torchOnState;

			// If the torch is being turned off, do it immediately.
			if (!currentStateData.torchOnState)
			{
				digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_NOT_ACTIVE);
			}
			// else If torch was turned on, let state machine logic decide
			// how to handle it.
		}

		//
		// READ THE ARC-GOOD STATE FROM THE PLASMA
		//
		ProcessArcGoodSignal();
        	if (currentStateData.arcStateChanged == true)
                  sendFullVoltageMessage();

		//
		// CHECK FOR COMMANDS FROM PC
		//
		// If there are serial characters present figure
		// out if there is a complete command packet, and
		// if so, process it.
		if (SERIAL.available() > 0)
		{
			uint8_t buff[10];
			uint16_t temp;

			// Look at the command (without "reading" it)
			switch (SERIAL.peek())
			{
			case PC_CMD_SYNC:
				// Read and throw away the sync byte.
				SERIAL.read();
				break;

			case PC_CMD_STEP_MODE:
				// Read and throw away the sync byte.
				SERIAL.read();
				handleModeChange();
				// Build and send the response.
				SendMode();
				break;

			case PC_CMD_GET_MODE:
				// Read the command byte and throw away.
				SERIAL.read();
				// Build and send the response.
				SendMode();
				break;

			case PC_CMD_STEP_UNIT:
				// Read the command byte and throw away.
				SERIAL.read();
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
				break;

			case PC_CMD_GET_UNIT:
				// Read the command byte and throw away.
				SERIAL.read();
				SendUnits();
				break;

			case PC_CMD_DISPLAY_ON_OFF:
				if (SERIAL.available() < 2)
					break;
				// Respond to the command.
				break;

			case PC_CMD_GET_STATUS:
				break;

			case PC_CMD_SET_SETPOINT:
				if (SERIAL.available() < 3)
					break;
				// Read the command byte and throw away.
				SERIAL.read();
				// Respond to the command.
				buff[0] = (uint8_t) SERIAL.read();
				buff[1] = (uint8_t) SERIAL.read();
				temp = (((uint16_t) buff[0]) << 8) | ((uint16_t) buff[1]);
				if (currentStateData.currentUnit == THC_UNIT_VOLTS)
					temp *= COUNTS_PER_VOLT;
				currentStateData.voltSetPoint = temp;
				SendVoltage(PC_RESP_SETPOINT);
				break;

			case PC_CMD_GET_SETPOINT:
				// Read the command byte and throw away.
				SERIAL.read();
				// Build and send the response.
				SendVoltage(PC_RESP_SETPOINT);
				break;

			case PC_CMD_GET_CURRENT:
				// Read the command byte and throw away.
				SERIAL.read();
				// Build and send the response.
				SendVoltage(PC_RESP_CURRENT);
				break;

			case PC_CMD_TEST_TORCH_ON:
				// Read the command byte and throw away.
				SERIAL.read();
				// Toggle the torch.
				TestTorchOn(true);
				break;

			case PC_CMD_TEST_TORCH_OFF:
				// Read the command byte and throw away.
				SERIAL.read();
				// Toggle the torch.
				TestTorchOn(false);
				break;

			case PC_CMD_CAPTURE_START:
				// Read the command byte and throw away.
				SERIAL.read();
				captureOn = true;
				sendVoltageCaptureState(captureOn);
				break;

			case PC_CMD_CAPTURE_STOP:
				// Read the command byte and throw away.
				SERIAL.read();
				captureOn = false;
				sendVoltageCaptureState(captureOn);
				break;

			case PC_CMD_CAPTURE_GET_STATE:
				// Read the command byte and throw away.
				SERIAL.read();
				sendVoltageCaptureState(captureOn);
				break;

			case PC_CMD_SETPOINT_UP:
				// Read the command byte and throw away.
				SERIAL.read();
				// Handle the up.
				if ((currentStateData.currentUnit == THC_UNIT_VOLTS)
						&& (currentStateData.voltSetPoint / COUNTS_PER_VOLT < 180))
				{
					currentStateData.voltSetPoint += COUNTS_PER_VOLT;
				}
				else if ((currentStateData.currentUnit == THC_UNIT_COUNTS)
						&& (currentStateData.voltSetPoint < 1020))
				{
					currentStateData.voltSetPoint++;
				}
				// Build and send the response.
				SendVoltage(PC_RESP_SETPOINT);
				// Update the LCD
				break;

			case PC_CMD_SETPOINT_DOWN:
				// Read the command byte and throw away.
				SERIAL.read();
				// Handle the down.
				if ((currentStateData.currentUnit == THC_UNIT_VOLTS)
						&& (currentStateData.voltSetPoint / COUNTS_PER_VOLT > 50))
				{
					currentStateData.voltSetPoint -= COUNTS_PER_VOLT;
				}
				else if ((currentStateData.currentUnit == THC_UNIT_COUNTS)
						&& (currentStateData.voltSetPoint > 350))
				{
					currentStateData.voltSetPoint--;
				}
				// Build and send the response.
				SendVoltage(PC_RESP_SETPOINT);
				// Update the LCD
				break;

			case PC_CMD_TEST_ARC_GOOD_ON:
				// Read the command byte and throw away.
				SERIAL.read();
				// Turn on arc good for test
				digitalWrite(OUT_D_ARC_GOOD, OUT_D_ARC_GOOD_ACTIVE);
				break;

			case PC_CMD_TEST_ARC_GOOD_OFF:
				// Read the command byte and throw away.
				SERIAL.read();
				// Turn off arc good for test
				digitalWrite(OUT_D_ARC_GOOD, OUT_D_ARC_GOOD_NOT_ACTIVE);
				break;

			case PC_CMD_TEST_THC_UP_ON:
				// Read the command byte and throw away.
				SERIAL.read();
				// Turn on torch up for test
				digitalWrite(OUT_D_TORCH_UP, OUT_D_TORCH_UP_ACTIVE);
				break;

			case PC_CMD_TEST_THC_UP_OFF:
				// Read the command byte and throw away.
				SERIAL.read();
				// Turn off torch up for test
				digitalWrite(OUT_D_TORCH_UP, OUT_D_TORCH_UP_NOT_ACTIVE);
				break;

			case PC_CMD_TEST_THC_DOWN_ON:
				// Read the command byte and throw away.
				SERIAL.read();
				// Turn on torch down for test
				digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_ACTIVE);
				break;

			case PC_CMD_TEST_THC_DOWN_OFF:
				// Read the command byte and throw away.
				SERIAL.read();
				// Turn off torch down for test
				digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_NOT_ACTIVE);
				break;

                        case PC_SET_COUNT_ON_HYSTERISIS:
                               break;
                               
                        case PC_SET_COUNT_OFF_HYSTERISIS:
                               break;
                               
                        case PC_SET_TORCH_STABILIZE_MILLISEC:
                               break;
                               
                        // Consider adding messages to set kerf threasdhold values.
                               

			default:
				break;
			}
		}

		//
		// RUN THE STATE MACHINE
		//

		// If in Disabled mode, don't do anything.
		if (currentStateData.currentMode == THC_MODE_DISABLED)
		{
			// Nothing is done when disabled.

			// Only update the display about 4 times a second (assuming a 1 ms loop time).
			if (++displayUpdateDelay > 250)
			{
				sendFullVoltageMessage();
				displayUpdateDelay = 0;
			}
		}
		// If in Bypass mode, use the handler that just passes signals through.
		else if (currentStateData.currentMode == THC_MODE_BYPASS)
		{
			//bypassHandler->controlLoop();

			// Only update the display about 4 times a second (assuming a 1 ms loop time).
			if (++displayUpdateDelay > 250)
			{
				sendFullVoltageMessage();
				displayUpdateDelay = 0;
			}

		}
		// If in Cruise mode, used the handler that just passes through
		// until the user hits the "Select" button to start the THC function.
		else if (currentStateData.currentMode == THC_MODE_CRUISE)
		{
			//bypassHandler->controlLoop();

			// Only update the display about 4 times a second (assuming a 1 ms loop time).
			if (++displayUpdateDelay > 250)
			{
				sendFullVoltageMessage();
				displayUpdateDelay = 0;
			}
		}
		// In the Enabled mode, do the standard full height control processing.
		else if (currentStateData.currentMode == THC_MODE_OPERATING)
		{
			//
			// RUN THE APPRORIATE HANDLER BASED ON THE STATE
			//

			// Run the state handler based on the current state,
			// either Enabled (waiting to cut) or cutting.
			if (currentStateData.currentState == THC_STATE_ENABLED)
			{
				// Enabled state handler returns true if usage data should be saved.
				if (StateEnabledHandler())
				{
					// Copy the updated data to the flash object
					//usageData.voltSetPoint = currentStateData.voltSetPoint;
					flash.DataSetAll((THCStatData *) &usageData);
					// Write the object data to the memory.
					flash.DataSave();
					// Clear the new data flag.
					currentStateData.usageUpdated = false;
					usageData.initialWriteCount++;
				}

      			    // Only update the display about 4 times a second (assuming a 1 ms loop time).
  			    if (++displayUpdateDelay > 250)
  			        {
    				  sendFullVoltageMessage();
  				  displayUpdateDelay = 0;
  			        }
			}

			else if (currentStateData.currentState == THC_STATE_CUTTING)
			{
				StateCuttingHandler();
                                if (captureOn)
                                  {
                                    sendFullVoltageMessage();
                                  }
			}



		} // State handler
	}  // While (1)

}	// loop()





void TestTorchOn(bool value)
{
	if (value)
		// Turn on torch for test
		digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_ACTIVE);
	else
		// Turn off torch for test
		digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_NOT_ACTIVE);

}

#if NOPE
// Check for keypress to start controlling cutting.
if (display.processCruiseButtons() == COMMAND_CRUISE_SET)
{
	if ( currentStateData.torchOnState && currentStateData.arcGoodState &&
			(currentStateData.currentVoltage > MIN_CRUISE_SET_VOLTAGE))
	{
		display.setMenu(MENU_OPERATING);
		//currentStateData.voltSetPoint = currentStateData.currentVoltage;
		currentStateData.currentMode = THC_MODE_ENABLED;
		currentStateData.currentState = THC_STATE_CUTTING;
		currentStateData.runVoltageControl = true;
	}
}
}
#endif

#if NOPE
void processCommand(Command_e newCmd)
{
// Process any command returned by the state handler.
switch (newCmd)
{
	default:
	case COMMAND_NONE:
	//SERIAL.print("COMMAND_NONE\n");
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
	usageData.initialWriteCount++;
	break;

}

}

#endif

#ifdef NOPE
// Only update the display about 20 times a second (assuming a 1 ms loop time).
//		if (++delayDisplayUpdate > 5)
{
sendFullVoltageMessage();
delayDisplayUpdate = 0;
}
#endif

