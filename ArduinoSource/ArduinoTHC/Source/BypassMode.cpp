/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * BypassMode.cpp
 *
 *  Created on: Dec 28, 2012
 *      Author: Rege
 */
#include "THC.h"
#include "IOConfig.h"
#include "THCInterface.hpp"

#include "BypassMode.hpp"

//
// Bypass Mode Constructor
//
// The constructor for Bypass Mode just needs to initialize its local
// variables.  Display pointer is passed in on creation so that it
// doesn't have to be passed in on every loop.
//
// Parameters:
//   THCInterface* - pointer to display object for display updates.
//
BypassMode::BypassMode(THCInterface *displayPtr)
	{
	torchOn = false;
	arcGood = false;
	// Copy the pointer to the display object.
	display = displayPtr;
	}




void BypassMode::controlLoop(void)
	{
	Command_e returnCmd = COMMAND_NONE;

	//
	// HANDLE THE TORCH ON/OFF CASES
	//

	// Handle the case of a torch on being newly requested.
	if ((currentStateData.torchOnState) && !torchOn)
		{
		// Set flag to denote we've turned the torch on.
		torchOn = true;

		SendCutStartPacket();

		// Start the use timer.
		currentStateData.cutStartTime.startTimer();

		// Trip the relay to turn the plasma torch on
		digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_ACTIVE);

		// Update the status display of the torch on signal
		display->TorchOn(currentStateData.torchOnState);

		// Update the pilot arc count since we're starting the torch
		// and update the flag to denote usage data is dirty.
		usageData.pilotArcCount++;
		currentStateData.usageUpdated = true;
		}

	// Handle the case of a torch OFF being newly requested
	else if ((torchOn) && (!currentStateData.torchOnState))
	{
		// Trip the relay to turn the plasma torch off
		digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_NOT_ACTIVE);

		// reset the flag
		torchOn = false;

		// Update the status display of the torch on signal
		display->TorchOn(currentStateData.torchOnState);

		// Start timer for delayed memory write
		currentStateData.lastUseTimer.startTimer();
	}

	// Handle the case of the torch running
	else if (torchOn && currentStateData.torchOnState)
		{
		// Do nothing
		}

	// Handle the case of the torch being off
	else if (!torchOn && !currentStateData.torchOnState)
		{
		// Since we're inactive, see if the usage data has been updated.
		if (currentStateData.usageUpdated)
			{
			display->setFlashWritePending(true);
			// To save write cycles, wait 30 seconds before updating the daa.
			if (currentStateData.lastUseTimer.elapsedSeconds() >= 30)
				{
				// Write the usage data
				returnCmd = COMMAND_SAVE_USAGE_DATA;
				}
			}
		}

	//
	// HANDLE THE ARC ON/OFF CASES
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
			// Update the pierce count since we finished a pierce.
			usageData.pierceCount++;

			// Start the timer that tracks torch on time.
			// (The ISR will update the use time when the arc goes off.)
			currentStateData.torchOnTimer.startTimer();
			}
		}

	}

