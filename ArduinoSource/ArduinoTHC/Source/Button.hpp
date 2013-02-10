/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * Button.hpp
 *
 * Description: The button class is used to handle Arduino push button inputs.
 *	It will provide debouncing and, if desired, auto repeating.
 *
 * Note: Use of the Button class requires that the I/O pin already be configured.
 *
 *  Created on: Sep 7, 2012
 *      Author: Rege George
 */
#ifndef BUTTON_HPP_
#define BUTTON_HPP_


#include "Timer.hpp"	// Timers are required for debouncing and auto-repeat


// Enumerated type to define repeating and non-repeating keys.
enum ButtonRepeat_e
    {
    BUTTON_REPEAT,
    BUTTON_NOREPEAT
    };


// Limit the length of button names to save RAM (size includes NULL)
#define BUTTON_NAME_LEN 11



class Button
    {

private:
    char buttonName[BUTTON_NAME_LEN - 1]; // Not currently used, but helpful for debugging

    int ioPin; 	     // Arduino Digital Input pin to read
    bool activeHigh; // Is the input pin Active High

    unsigned int debounceTimeMs; 	// Milliseconds to debounce presses
    unsigned int repeatStartDelayMs;	// How long key must be held down before auto-repeat starts
    unsigned int repeatEveryMs; 	// How often key repeats once repeating starts.

    Timer debounceTime, holdTime; 	// Timers for debouncing press and repeating key presses.
    unsigned int repeatCount; 		// Tracks the number of presses, needed for auto-repeat.

    bool isPressedNow; 			// Used by "DebounceButton()" to track if press has been reported
    bool pressReported; 		// Used by "WasPressed()" to track if press was reported

    Button(); 			// Make the default constructor private so that it can't be used.
    bool DebounceButton(); 	// Internal routine to debounce the key presses.

public:

    Button(
	    char *name,
	    int pin, bool paramActiveHigh, unsigned int paramDebounceTimeMs,
	    unsigned int paramRepeatDelayMs, unsigned int paramRepeatEveryMs);

    bool WasPressed(ButtonRepeat_e repeatStatus); // Called to check for key press, flag used to
						  // denote if repeating presses are desired.

    };

#endif /* BUTTON_HPP_ */
