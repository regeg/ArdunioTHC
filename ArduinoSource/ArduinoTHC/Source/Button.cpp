/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * Button.cpp
 *
 *  Created on: Sep 7, 2012
 *      Author: Rege George
 *
 */

#include "Arduino.h"
#include "HardwareSerial.h"

#include "Button.hpp"
#include "IOConfig.h"
#include "IO.hpp"




// Button Constructor
//
// The constructor requires that you define all the important
// stuff for a button.  It requires values for software debounce
// and auto-repeat, even if you don't use them.
//
// Parameters:
//	name - button name that is up to 10 characters long
//	pin - Arduino digial I/O pin number (must already be configured)
//	paramActiveHigh - is the pin's signal active when high?
//	paramDebounceTimeMs - milliseconds to debounce the key on press
//	paramRepeatStartDelayMs - milliseconds to wait to start repeating the key
//		if repeating is requested (must be great than debounce time)
//	paramRepeatEveryMs - if repeating, how many milliseonds to wait to signal
//		another key press.
//
Button::Button(char *name,
	       int pin, bool paramActiveHigh,
	       unsigned int paramDebounceTimeMs,
	       unsigned int paramRepeatStartDelayMs,
	       unsigned int paramRepeatEveryMs)
    {

    // Copy the button name, truncating it if necessary
    strncpy(buttonName, name, BUTTON_NAME_LEN);

    // Get the I/O pin to be read and if its active high.
    ioPin = pin;
    activeHigh = paramActiveHigh;

    debounceTimeMs = paramDebounceTimeMs;

    // Make sure that the repeat start delay time is at least twice
    // the debounce time. If it's not, set it (add 1 in case debounce is 0)
    if (paramRepeatStartDelayMs < debounceTimeMs)
	repeatStartDelayMs = (debounceTimeMs*2) + 1;
    else
	repeatStartDelayMs = paramRepeatStartDelayMs;

    // Make sure that the repeat every is at least 1 (0 breaks the math).
    if (paramRepeatEveryMs == 0 )
	repeatEveryMs = 1;
    else
	repeatEveryMs = paramRepeatEveryMs;

    // Clear the flags and counters.
    isPressedNow = false;
    pressReported = false;
    repeatCount = 0;
    }




//
// DebounceButton()
//
// This is the lowest level button I/O routine.  It is used to
// read the I/O, set the input value to the appropriate logic
// level based on it's active high/low state, and debounce
// the button.
//
// Parameters:
//	None
//
// Returns:
//	bool - true if debounce button status is pressed
//
bool Button::DebounceButton()
    {
    unsigned int currentState;

    // Read the value and adjust it based on the active high/low state.
    currentState = decodeLogicLevel(digitalRead(ioPin), activeHigh);

    // Was it just released after being pressed?
    if (isPressedNow && (currentState == 0))
	{
	// Ignore off's until debounce time expired
	if (debounceTime.elapsedMilliSeconds() < debounceTimeMs)
	    return true;

	// It's been debounced, so it can be set to off now.
	isPressedNow = false;
	return false;
	}
    else if (isPressedNow && (currentState == 1))
	return true;

    // Was it pressed for the first time?
    else if (!isPressedNow && (currentState == 1))
	{
	// You don't need to debounce for the first press since the
	// release is debounced.
	debounceTime.startTimer();
	isPressedNow = true;
	return true;
	}

    // If neither of the above is true, then the state hasn't changed.
    return false;
    }




//
// WasPressed()
//
// This routine will read the button hardware and "generate a press event"
// the first time a button is pressed.  After a press is reported once
// it will not be reported again unless looking for repeating presses.
//
// Note:
//	If a button is pressed and released between calls to this routine
//	it will not be detected.
//
// Parameters:
//	repeatState - denotes if this call wants to detect held button as repeat
//
// Returns:
//	bool -	true is button is pressed and repeat status is BUTTON_NOREPEAT or
//		if the button is held down and the status is BUTTON_REPEAT
//
bool Button::WasPressed(ButtonRepeat_e repeatStatus)
    {
    unsigned long int currentHoldTime;
    unsigned int intervals;

    // See what the button's current state is.
    if (!DebounceButton())
	{
	// Button is not currently pressed, clear flag.
	pressReported = false;
	return false;
	}

    // At this point, we know the button is pressed.

    // Is it being pressed for the first time?  If so report it and set
    // the flag to track it was reported.
    if (!pressReported)
	{
	// Start the repeat timer.
	holdTime.startTimer();
	// Reset the repeat count
	repeatCount = 0;
	pressReported = true;
	return true;
	}

    // At this point, we know the button is pressed and
    // the first press event has been reported.

    // See if we want to repeat the key if its being held down
    if ( (repeatStatus == BUTTON_REPEAT) && (repeatEveryMs != 0) )
	{
	// We only want to repeat the key press as often
	// as specified when the object was created.

	// See how long the key has been held down.
	currentHoldTime = holdTime.elapsedMilliSeconds();

	// First, make sure the initial hold time has completed
	// before starting the repeating.
	if (currentHoldTime < repeatStartDelayMs)
	    return false;

	// We now know that the initial key hold has expired
	// so that we can start repeating the key

	// To see if we need to repeat calculate how many
	// key presses should have been generated based on
	// how long its been held down.
	intervals = (unsigned int) ((currentHoldTime-repeatStartDelayMs) / repeatEveryMs);

	// If we haven't reported enough key repeats, then return a keypress.
	if (intervals > repeatCount)
	    {
	    repeatCount++;
	    return true;
	    }
	}

    // We're not interested in repeating the key press
    // so return false even though its being held down.
    return false;
    }

