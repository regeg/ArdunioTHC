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
#include "Timer.h"
#include "NonVolatile.h"
#include "Messages.h"

#include "StateEnabled.h"

static unsigned int displayUpdateDelay = 0;

//
// Main state handler.
//
bool StateEnabledHandler()
	{
	// Only update the display about 4 times a second (assuming a 1 ms loop time).
	if (++displayUpdateDelay > 250)
	{
		sendFullVoltageMessage();
		displayUpdateDelay = 0;
	}

	// Since we're not cutting, see if the usage data has been updated.
	if (currentStateData.usageUpdated)
		{
		// To save write cycles, wait 30 seconds before updating the daa.
		if (currentStateData.lastUseTimer.elapsedSeconds() >= 30)
			{
			// Write the usage data
			return true;
			}
		}

	// See if a request was received to turn the torch on.
	if (currentStateData.torchOnState)
		{
		currentStateData.cutStartTime.startTimer();
		// We were idle and the torch was turned on.
		// Trip the relay to turn the plasma torch on
		digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_ACTIVE);
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

                // Send message to update mode display
                SendMode();

		// return done, so return.
		return false;
		}

	// See if the Arc Ok signal changed so display can be updated.
	// The only change in ENABLED should be when cutting and
	// the torch is turned off, the ARC Good signal going off
	// should lag slightly.
	if (currentStateData.arcStateChanged)
		{
		// Clear the flag to denote we handled the state update.
		currentStateData.arcStateChanged = false;
		}

	// We're still in idle, so see if any new commands were entered and return them.
	return false;
	}

