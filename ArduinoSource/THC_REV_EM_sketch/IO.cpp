/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * IO.cpp
 *
 * This file continues the routines to initialize the IO and to
 * process the Arc Good signal from the plasma.
 *
 *  Created on: Sep 22, 2012
 *      Author: Rege George
 */
#include "Arduino.h"

#include "THC.h"
#include "IOConfig.h"
#include "IO.h"
#include "Timer.h"


//
// setupIO()
//
// Initialize all hardware for the I/O signals.
//
void setupIO(void)
	{

	// Define the pin for the TORCH_ON and ARC_GOOD signals.
	// These will be interrupts to ensure they are responded
	// to in a timely manner.
	pinMode(IN_D_TORCH_ON_PIN, INPUT_PULLUP);
	pinMode(IN_D_ARC_GOOD_PIN, INPUT_PULLUP);

	// Define the control signal output pins
	pinMode(OUT_D_TORCH_RELAY, OUTPUT); // To relay that turns plasma on
	pinMode(OUT_D_TORCH_UP, OUTPUT);// To CNC to signal the torch needs to go up
	pinMode(OUT_D_TORCH_DOWN, OUTPUT);// To CNC to signal the torch needs to go up
	pinMode(OUT_D_ARC_GOOD, OUTPUT);// To CNC to signal the cutting arc is good

	//
	// Initialize all the control signals.
	//
	digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_NOT_ACTIVE);// "Torch On" should be off by default.
	digitalWrite(OUT_D_TORCH_UP, OUT_D_TORCH_UP_NOT_ACTIVE);// "Torch Up" signal should be off by default
	digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_NOT_ACTIVE);// "Torch Down" signal should be off by default
	digitalWrite(OUT_D_ARC_GOOD, OUT_D_ARC_GOOD_NOT_ACTIVE);// "Arc Good" signal should be off by default

	// Read the current state of the "Torch On" signal and set the global variable
	currentStateData.torchOnState = (
			digitalRead(IN_D_TORCH_ON_PIN) == IN_D_TORCH_ON_PIN_ACTIVE ?
					true : false);

	// Read the current state of the "Arc Good" and set the global variable
	currentStateData.arcGoodState = (
			digitalRead(IN_D_ARC_GOOD_PIN) == IN_D_ARC_GOOD_PIN_ACTIVE ?
					true : false);
	}




//
// arcGoodIsr()
//
// This interrupt service Routine is called whenever the "Arc Good" signal
// changes state (off to on, or on to off).
//
// HACK NOTE: I think I might be getting spurious interrupts, so until I
//	      determine what's going on, put in a check to see if the
//	      signal actually changed.
//
void ProcessArcGoodSignal(void)
	{
	bool temp;

	// Read the current and correct for active state.
	temp = (digitalRead(IN_D_ARC_GOOD_PIN) ==
			IN_D_ARC_GOOD_PIN_ACTIVE) ? true : false;

	// If nothing changed, exit.
	if (currentStateData.arcGoodState == temp)
		return;

	// We know the state has changed, so update it.
	currentStateData.arcGoodState = temp;

	// Take action based on the new state.
	if (currentStateData.arcGoodState)
		{
		// Signal to the CNC that the Arc is Good.
		digitalWrite(OUT_D_ARC_GOOD, OUT_D_ARC_GOOD_ACTIVE);
		// The arc is now good, so start the cut timer.
		currentStateData.torchOnTimer.startTimer();
		// The arc just went good, so start the timer that allows
		// the torch to stabilize before we start voltage control.
		currentStateData.torchStabilizeTimer.startTimer();
		}
	else
		{
		// Signal to the CNC that the Arc is no longer active.
		digitalWrite(OUT_D_ARC_GOOD, OUT_D_ARC_GOOD_NOT_ACTIVE);
		// Turn off the flag for voltage control.
		currentStateData.runVoltageControl = false;
		// Ad the amount of time the torch ran to the tip use timer.
		AddSecondsToTipUse((THCStatData *) &usageData,
				currentStateData.torchOnTimer.elapsedSeconds());
		// Set the flag so the updated usage is written to flash.
		currentStateData.usageUpdated = true;
		}

	// Set the flag to indicate that the arc state has changed.
	currentStateData.arcStateChanged = true;
	}

