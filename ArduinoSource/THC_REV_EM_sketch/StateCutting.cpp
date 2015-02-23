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

#include "IOConfig.h"
#include "Timer.h"
#include "Messages.h"

#include "StateCutting.h"

// 5 volts (35 counts) is the immediate threshold.
#define KERF_ENCOUNTERED_THRESHOLD (currentStateData.voltSetPoint + (35))
#define KERF_ENCOUNTERED_DELAYED_THRESHOLD (currentStateData.voltSetPoint + (VOLTAGE_ON_HYSTERESIS*2))
#define KERF_CLEARED_THRESHOLD (currentStateData.voltSetPoint + (VOLTAGE_ON_HYSTERESIS*2))



void TorchUp()
	{
	// Turn on TorchUp
	digitalWrite(OUT_D_TORCH_UP, OUT_D_TORCH_UP_ACTIVE);
	// Make sure TorchDown is off
	digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_NOT_ACTIVE);
	// Set the flags for torch status.
	currentStateData.torchDown = false;
	currentStateData.torchUp = true;
	}




void TorchDown()
	{
	// Make sure TorchUp is off
	digitalWrite(OUT_D_TORCH_UP, OUT_D_TORCH_UP_NOT_ACTIVE);
	// Turn on TorchDown
	digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_ACTIVE);
	// Set the flags for torch status.
	currentStateData.torchDown = true;
	currentStateData.torchUp = false;
        // Start the kerf timer.
        currentStateData.kerfTimer.startTimer();
	}




void TorchKerf()
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
	}




void TorchKerfCrossed()
	{
	currentStateData.kerfEncountered = false;
	currentStateData.runVoltageControl = true;
	}




void TorchGood()
	{
	// Make sure TorchUp is off
	digitalWrite(OUT_D_TORCH_UP, !OUT_D_TORCH_UP_ACTIVE);
	// Make sure TorchDown is off
	digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_NOT_ACTIVE);
	// Set the flags for torch status.
	currentStateData.torchDown = false;
	currentStateData.torchUp = false;
	currentStateData.kerfEncountered = false;
	currentStateData.runVoltageControl = true;
	}


void SignalUpdateForTorchOrArcOff()
	{
	// Make sure TorchUp is off
	digitalWrite(OUT_D_TORCH_UP, !OUT_D_TORCH_UP_ACTIVE);
	// Make sure TorchDown is off
	digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_NOT_ACTIVE);
	// Set the flags for torch status.
	currentStateData.torchDown = false;
	currentStateData.torchUp = false;
	currentStateData.kerfEncountered = false;
	currentStateData.runVoltageControl = false;
	}


void StateCuttingHandler()
	{
        int cutVoltDelta;
        
	//
	// Check for ARC GOOD changes
	//

	// See if the Arc Ok signal changed so display can be updated.
	if (currentStateData.arcStateChanged)
		{
		// Clear the flag to denote we handled the state update.
		currentStateData.arcStateChanged = false;
		// If the arc good went on, update the usage data.
		if (currentStateData.arcGoodState)
			{
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
		// Must ensure torch up/down off if torch off.
                SignalUpdateForTorchOrArcOff();

		// Change the state from cutting to enabled.
		currentStateData.currentState = THC_STATE_ENABLED;

		// Start timer for delayed memory write
		currentStateData.lastUseTimer.startTimer();

                // Update the mode display
                SendMode();

		// Return now with no new command to process.
		return;
		}

	//
	// Check for the arc going out.
	//
	if (!currentStateData.arcGoodState)
		{
		// Reset control flags
                SignalUpdateForTorchOrArcOff();
		return;
		}

	//
	// If voltage control is not active, see if it should be turned on.
	//
	if (!currentStateData.runVoltageControl)
		{
		// Handle the case where voltage control is off due to a kerf being crossed.
		if (currentStateData.kerfEncountered)
                    {
                     if (currentStateData.currentVoltage <= (currentStateData.voltSetPoint + (2 * VOLTAGE_ON_HYSTERESIS)) )
                        // Turn voltage control back on
                        // Set flag to "no kerf"
                        TorchKerfCrossed();
                     else
                       // Still in the kerf, so don't do anything.
                       return;
                    }

		// This is the initial start of cut case, so see if the timer has expired.
		else if (currentStateData.torchStabilizeTimer.elapsedMilliSeconds() > TORCH_STABILIZE_TIME_MLLISECONDS)
			{
			currentStateData.runVoltageControl = true;
			}
                else
        	   // Thre is no need to do voltage control at this time.
        	   return;
		}


	//
	// Run the voltage control.
	//

	// One of the following is done:
	//	1) See if the voltage is too low, and if so turn on Torch Up
	//	1) if Torch is going Up, see if high enough and turn off if needed
	//	2) a) if Torch is going Down, see if low enough and turn off if needed
	//     b) see if we're crossing a kerf, and if so, turn off voltage control
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
		if (currentStateData.currentVoltage >=
			(currentStateData.voltSetPoint - VOLTAGE_OFF_HYSTERESIS))
			{
			// Handle case of torch at height.
			TorchGood();
			}
		}

	//
	// See if the voltage is too low, and if so, move the torch up.
	//
	else if (currentStateData.currentVoltage <=
			(currentStateData.voltSetPoint - VOLTAGE_ON_HYSTERESIS))
		// Update display that torch needs to go up
		TorchUp();

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
			TorchGood();
			}

                // Test for encountering a Kerf
		else if (!currentStateData.kerfEncountered)
                        {
                             // Give it some time to see if the slope of the voltage climb is too high,
                             // or if we've exeeded the cut voltage by 3 volts.  If so, we're on a kerf.
                             // At 100 ipm with a 0.06 kerf width, you will be halfway across in 18 ms.
                             if(currentStateData.kerfTimer.elapsedMilliSeconds() > 12)
                             {
                             // Determine how many counts we're over the setpoint.
                             cutVoltDelta = currentStateData.currentVoltage - currentStateData.voltSetPoint;
                             // See if the slope is too high or we've climbed too high.
                             if ( (((cutVoltDelta * 10) / currentStateData.kerfTimer.elapsedMilliSeconds()) >= 10) ||
                                   (cutVoltDelta > (COUNTS_PER_VOLT * 3)) )
                                   // Kerf encountered.
                                   TorchKerf();
                             }
                        }
		}
        // Test to see if the torch needs to go down.
	else
        {
           // Check to see if the torch is to high.
	   if (currentStateData.currentVoltage >=
 	      (currentStateData.voltSetPoint + VOLTAGE_ON_HYSTERESIS))
	      {
	      // Handle case of torch to high.
	      TorchDown();
              }
	}

}




