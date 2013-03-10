/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * StateCutting.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: Rege
 */
#include "THC.h"
#include "Timer.hpp"

#include "HardwareSerial.h"

//#include "THCInterface.hpp"
#include "IOConfig.h"
#include "Timer.hpp"

#include "StateCutting.hpp"


void TorchUp(THCInterface *display)
	{
	// Turn on TorchUp
	digitalWrite(OUT_D_TORCH_UP, OUT_D_TORCH_UP_ACTIVE);
	// Make sure TorchDown is off
	digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_NOT_ACTIVE);
	// Set the flags for torch status.
	currentStateData.torchDown = false;
	currentStateData.torchUp = true;
	// Update the display with the torch control status
	display->SetTorchState(TORCH_STATE_UP);
	}

void TorchDown(THCInterface *display)
	{
	// Make sure TorchUp is off
	digitalWrite(OUT_D_TORCH_UP, OUT_D_TORCH_UP_NOT_ACTIVE);
	// Turn on TorchDown
	digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_ACTIVE);
	// Set the flags for torch status.
	currentStateData.torchDown = true;
	currentStateData.torchUp = false;
	// Update the display with the torch control status
	display->SetTorchState(TORCH_STATE_DOWN);
	}


void TorchKerf(THCInterface *display)
	{
	// Make sure TorchUp is off
	digitalWrite(OUT_D_TORCH_UP, !OUT_D_TORCH_UP_ACTIVE);
	// Make sure TorchDown is off
	digitalWrite(OUT_D_TORCH_DOWN, !OUT_D_TORCH_DOWN_ACTIVE);
	// Set the flags for torch status.
	currentStateData.torchDown = false;
	currentStateData.torchUp = false;
	currentStateData.kerfEncountered = true;
	currentStateData.runVoltageControl = false;
	// Update the display with the torch control status
	display->SetTorchState(TORCH_STATE_KERF);
	}

void TorchKerfCrossed(THCInterface *display)
	{
	currentStateData.kerfEncountered = false;
	currentStateData.runVoltageControl = true;
	}


void TorchGood(THCInterface *display)
	{
	// Make sure TorchUp is off
	digitalWrite(OUT_D_TORCH_UP, !OUT_D_TORCH_UP_ACTIVE);
	// Make sure TorchDown is off
	digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_NOT_ACTIVE);
	// Set the flags for torch status.
	currentStateData.torchDown = false;
	currentStateData.torchUp = false;
	// Update the display with the torch control status
	display->SetTorchState(TORCH_STATE_GOOD);
	}





Command_e StateCuttingHandler(THCInterface *display)
	{

	//
	// Check for ARC GOOD changes
	//

	// See if the Arc Ok signal changed so display can be updated.
	if (currentStateData.arcStateChanged)
		{
		// Update the display with the current state.
		display->ArcGood(currentStateData.arcGoodState);
		// Clear the flag to denote we handled the state update.
		currentStateData.arcStateChanged = false;
		// If the arc good went on, update the usage data.
		if (currentStateData.arcGoodState)
			{
			//Serial1.print(currentStateData.cutStartTime.elapsedMilliSeconds());
			//Serial1.println(": arc on");
			// Update the pierce count since we finished a pierce.
			usageData.pierceCount++;
			// Update the flag so the data will be flushed later.
			currentStateData.usageUpdated = true;
			// Start the timer that tracks torch on time.
			currentStateData.torchOnTimer.startTimer();
			}
		}

	//
	// Check for the TORCH ON signal being turned off.
	//
	if (!currentStateData.torchOnState)
		{
		// Turn off the relay so that the torch on signal to the plasma goes off.
		digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_NOT_ACTIVE);
		//Serial1.print(currentStateData.cutStartTime.elapsedMilliSeconds());
		//Serial1.println(": torch off");
		// Must ensure torch up/down off if torch off..
		TorchGood(display);

		// Update the status display of the torch on signal
		display->TorchOn(currentStateData.torchOnState);

		// Change the state from cutting to enabled.
		currentStateData.currentState = THC_STATE_ENABLED;

		// Start timer for delayed memory write
		currentStateData.lastUseTimer.startTimer();

		// Reset control flags
		currentStateData.runVoltageControl = false;
		currentStateData.kerfEncountered = false;

		// Return now with no new command to process.
		return COMMAND_NONE;
		}

	//
	// Check for the arc going out.
	//
	if (!currentStateData.arcGoodState)
		{
		// Reset control flags
		// Turn off the voltage control and reset the kerf crossing flag.
		currentStateData.runVoltageControl = false;
		currentStateData.kerfEncountered = false;
		return COMMAND_NONE;
		}

	//
	// If voltage control is not active, see if it should be turned on.
	//
	if (!currentStateData.runVoltageControl)
		{
		// Handle the case where voltage control is off due to a kerf being crossed.
		if (currentStateData.kerfEncountered)
			{
			if (currentStateData.kerfVoltage >= currentStateData.currentVoltage)
				// The voltage has returned to a "pre-kerf" level, so resume control.
				TorchKerfCrossed(display);
			}

		// This is the initial start of cut case, so see if the timer has expired.
		else if (currentStateData.torchStabilizeTimer.elapsedMilliSeconds() > TORCH_STABILIZE_TIME_MLLISECONDS)
			{
			currentStateData.runVoltageControl = true;
			}

		// For code clarity, return now and start voltage control next pass (its only 1 ms), if needed.
		return COMMAND_NONE;
		}


	//
	// Run the voltage control.
	//

	// One one of the following is done, but the selection is in this order:
	//	1) if Torch is going Up, see if high enough and turn off if needed
	//	2) a) if Torch is going Down, see if low enough and turn off if needed
	//     b) see if we're crossing a kerf, and if so, turn off voltage control
	//	3) See if the voltage is too low, and if so turn on Torch Up
	//	4) See if the voltage is too high, and if so turn on Torch Down
	//
	// After torch height control is done, see if the user it trying to
	// adjust the tip voltage.

	//
	// Handle case of where Torch Up is active
	//
	if (currentStateData.torchUp)
		{
		// Check to see if the torch is high enough.
		if (currentStateData.currentVoltage	>=
			(currentStateData.voltSetPoint - VOLTAGE_OFF_HYSTERESIS))
			{
			// Handle case of torch at height.
			TorchGood(display);
			}
		}

	//
	// Handle the case where Torch Down is active
	//
	else if (currentStateData.torchDown)
		{
		// Check to see if the torch is low enough.
		if (currentStateData.currentVoltage <=
			(currentStateData.voltSetPoint + VOLTAGE_OFF_HYSTERESIS))
			{
			// Handle case of torch at height.
			TorchGood(display);
			}
		else // Check to see if we're crossing a kerf
			{
			// We consider that we're crossing a kerf if either:
			// a) voltage goes 2 above the voltage when torch up was detected, or
			// b) after 7 ms. the voltage has not gone down.
			if ((currentStateData.currentVoltage > (currentStateData.kerfVoltage+1)) ||
				((currentStateData.kerfTimer.elapsedMilliSeconds()> 7) &&
				 (currentStateData.currentVoltage >= (currentStateData.kerfVoltage))) )
				{
				// Assume we've encountered a kerf, call routine to set all flags.
				TorchKerf(display);
				}
			}
		}

	//
	// See if the voltage is too low, and if so, move the torch up.
	//
	else if (currentStateData.currentVoltage <=
			(currentStateData.voltSetPoint - VOLTAGE_ON_HYSTERESIS))
		// Update display that torch needs to go up
		TorchUp(display);

	//
	// See if the voltage is too high, and if so move the torch down.
	//
	else if ( (currentStateData.currentVoltage >=
			  (currentStateData.voltSetPoint + VOLTAGE_ON_HYSTERESIS)) &&
			!currentStateData.kerfEncountered)
		{
		// Update display that torch needs to go up
		TorchDown(display);
		// Set flags to signal torch down.
		currentStateData.torchUp = false;
		currentStateData.torchDown = true;
		currentStateData.kerfVoltage = currentStateData.currentVoltage;
		currentStateData.kerfTimer.startTimer();
		}

	// Only process the voltage up/down buttons since we're cutting.
	return display->processUpDownButtons();
	}




