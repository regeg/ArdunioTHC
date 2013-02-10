/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * Isr.cpp
 *
 * This file continues the routines to initialize the IO and to handle the
 * interrupts for Plasma and CNC signals.
 *
 *  Created on: Sep 22, 2012
 *      Author: Rege George
 */
#include "Arduino.h"

#include "THC.h"
#include "IOConfig.h"
#include "THCInterface.hpp"
#include "IO.hpp"
#include "Timer.hpp"

//
// decodeLogicLevel
//
// Convenience function to determine value of signal based
// on being active high or active low.
//
// Parameters:
//	currentLevel - value from digital read (0 or 1)
//	activeLevel - HIGH or LOW
//
// Returns:
//	1 - if signal is active based on current value and input sense
//	0 - if signal is inactive based on current value and input sense
//
// Note: Function is "in-lined" because we have plenty of program space and
//	limited time/RAM.
//
unsigned int decodeLogicLevel(int currentLevel, bool activeHigh)
	{
	// If the signal needs inverted, do it.
	if (activeHigh == false)
		return ((currentLevel == 0) ? 1 : 0);

	// Return what was passed in since it doesn't need inverted.
	return currentLevel;
	}

//
// setupIO()
//
// Initialize all hardware for the I/O signals.
//
void setupIO(void)
	{
	// Define the input for all the push buttons
	pinMode(IN_D_BUTTON_SELECT, INPUT_PULLUP);
	pinMode(IN_D_BUTTON_CANCEL, INPUT_PULLUP);
	pinMode(IN_D_BUTTON_UP, INPUT_PULLUP);
	pinMode(IN_D_BUTTON_DOWN, INPUT_PULLUP);
	pinMode(IN_D_BUTTON_MODE, INPUT_PULLUP);

	// Define the pin for the TORCH_ON and ARC_GOOD signals.
	// These will be interrupts to ensure they are responded
	// to in a timely manner.
	pinMode(IN_D_INT_TORCH_ON_PIN, INPUT_PULLUP);
	pinMode(IN_D_INT_ARC_GOOD_PIN, INPUT_PULLUP);

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

	//
	// Set up the interrupts for the "Torch On" and "Arc Good" signals.
	// We want change so we get interrupted on every change.
	//
	attachInterrupt(VECTOR_TORCH_ON_ISR, torchOnIsr, CHANGE);
	attachInterrupt(VECTOR_ARC_GOOD_ISR, arcGoodIsr, CHANGE);

	// Read the current state of the "Torch On" signal and set the global variable
	currentStateData.torchOnState = (
			digitalRead(IN_D_INT_TORCH_ON_PIN) == IN_D_TORCH_ON_PIN_ACTIVE ?
					true : false);

	// Read the current state of the "Arc Good" and set the global variable
	currentStateData.arcGoodState = (
			digitalRead(IN_D_INT_ARC_GOOD_PIN) == IN_D_ARC_GOOD_PIN_ACTIVE ?
					1 : 0);
	}

//
// torchOnIsr()
//
// This Interrupt Service Routine is called whenever the "Torch On" signal
// changes state (off to on, or on to off).
//
// SAFETY NOTE: From a safety viewpoint, if a "Torch Off" is received, always do
//		if from the ISR in case the main loop is hosed.  Don't turn
//		on the torch since the main loop has logic to determine if that
//		should be allowed.
//
void torchOnIsr(void)
	{
	// Read the current state of the torch on request and
	// invert the state of the signal since it's active low.
	currentStateData.torchOnState = (
			digitalRead(IN_D_INT_TORCH_ON_PIN) == IN_D_TORCH_ON_PIN_ACTIVE ?
					true : false);

	// If the torch is being turned off, do it immediately.
	if (!currentStateData.torchOnState)
		{
		digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_NOT_ACTIVE);
		}
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
void arcGoodIsr(void)
	{
	bool temp;

	// Read the current and correct for active state.
	temp = decodeLogicLevel(digitalRead(IN_D_INT_ARC_GOOD_PIN),
			IN_D_ARC_GOOD_PIN_ACTIVE);

	// Hack: I think this is very noisy, so ignore if no changes
	if (temp == currentStateData.arcGoodState)
		return;

	// Set the global to the new value.
	currentStateData.arcGoodState = temp;

	// For now, just pass the arc state through.  Logic may
	// be added in the future and this would be removed.
	if (currentStateData.arcGoodState)
		{
		digitalWrite(OUT_D_ARC_GOOD, OUT_D_ARC_GOOD_ACTIVE);
		currentStateData.torchOnTimer.startTimer();
		currentStateData.torchStabilizeTimer.startTimer();
		}
	else
		{
		digitalWrite(OUT_D_ARC_GOOD, OUT_D_ARC_GOOD_NOT_ACTIVE);
		currentStateData.runVoltageControl = false;
		AddSecondsToTipUse((THCStatData *) &usageData,
				currentStateData.torchOnTimer.elapsedSeconds());
		}

	currentStateData.usageUpdated = true;
	currentStateData.arcStateChanged = true;
	}
