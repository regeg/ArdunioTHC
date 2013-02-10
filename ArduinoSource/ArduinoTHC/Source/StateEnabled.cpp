/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * StateEnabled.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: Rege
 */

#include "THC.h"

#include "HardwareSerial.h"

#include "IOConfig.h"
#include "Timer.hpp"
#include "NonVolatile.hpp"
#include "StateEnabled.hpp"

Command_e StateEnabledHandler(THCInterface *display)
	{

	// If we're inactive, see if the usage data has been updated.
	if (currentStateData.usageUpdated)
		{
		display->setFlashWritePending(true);
		// To save write cycles, wait 30 seconds before updating the daa.
		if (currentStateData.lastUseTimer.elapsedSeconds() >= 30)
			{
			// Write the usage data
			return COMMAND_SAVE_USAGE_DATA;
			}
		}

	// See if a request was received to turn the torch on.
	if (currentStateData.torchOnState)
		{
		currentStateData.cutStartTime.startTimer();
		//Serial1.print(currentStateData.cutStartTime.elapsedMilliSeconds());
		//Serial1.println(": Torch On");
		// We were idle and the torch was turned on.
		// Trip the relay to turn the plasma torch on
		digitalWrite(OUT_D_TORCH_RELAY, HIGH);
		// Update the pilot arc count since we're starting the torch
		// and update the flag to denote usage data is dirty.
		usageData.pilotArcCount++;
		currentStateData.usageUpdated = true;

		// Change the state to CUTTING
		currentStateData.currentState = THC_STATE_CUTTING;

		// Make sure voltage control is off.
		currentStateData.runVoltageControl = false;

		currentStateData.torchUp = false;
		currentStateData.torchDown = false;

		// If we were in any other menu, then revert to the cutting display
		display->setMenu(MENU_OPERATING);
		// return done, so return.
		return COMMAND_NONE;
		}

	// See if the Arc Ok signal changed so display can be updated.
	// The only change in ENABLED should be when cutting and
	// the torch is turned off, the ARC Good signal going off
	// should lag slightly.
	if (currentStateData.arcStateChanged)
		{
		//Serial1.print(millis());
		//if (currentStateData.arcGoodState)
		//    Serial1.println(": arc good");
		//else
		//    Serial1.println(" arc bad");

		// Update the display with the current state.
		display->ArcGood(currentStateData.arcGoodState);
		if (!currentStateData.arcGoodState)
			display->SetTorchState(TORCH_STATE_NONE);

		// Clear the flag to denote we handled the state update.
		currentStateData.arcStateChanged = false;
		}

	// We're still in idle, so see if any new commands were entered and return them.
	return display->processButtons();
	}
