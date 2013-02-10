/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * Timer.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: Rege George
 */

#include "Arduino.h"

#include "Timer.hpp"




//
// Timer Constructor
//
// This is the default constructor.  You can't really address problems
// if the user doesn't call "startTimer()" before checking the time,
// but the constructor will initialize the start time to 0 anyways.
//
Timer::Timer()
    {
    // Initialize the start time to 0 in case "startTimer()" is
    // never called.
    startTimeMillis = 0;
    }




//
// startTimer()
//
// Record the current time as the "start" time for the timer.
//
void Timer::startTimer()
{
    // Use the Arduino system timer to get the time in milliseconds.
    startTimeMillis = millis();
}




//
// elapsedSeconds()
//
// This routine will determine the number of elapsed seconds since the
// "startTimer()" call.  It will only address one timer wrap.
//
// Note: There is no way to detect more than one timer wrap around, but
//	shouldn't be an issue since it takes about 50 days to wrap.
//
// Parameters:
//	None
//
// Returns:
//	ulong - number of seconds elapsed since timer started.
//
unsigned long Timer::elapsedSeconds()
{
  unsigned long currentTime;

  // Get the current millisecond time.
  currentTime = millis();

  // Figure out if a system timer wrap must be accounted for.
  if (currentTime < startTimeMillis)
    return ((0xffffffff - startTimeMillis + currentTime) / 1000);
  else
    return((currentTime - startTimeMillis) / 1000);
}




//
// elapsedMilliSeconds()
//
// This routine will determine the number of elapsed milliseconds since
// the "startTimer()" call.  It will address timer wraps.
//
// Note: There is no way to detect more than one timer wrap around, but
//	shouldn't be an issue since it takes about 50 days to wrap.
//
// Parameters:
//	None
//
// Returns:
//	ulong - number of seconds elapsed since timer started.
//
unsigned long Timer::elapsedMilliSeconds()
{
  unsigned long currentTime;

  // Get the current millisecond time.
  currentTime = millis();

  // Figure out if a wrap must be done.
  if (currentTime < startTimeMillis)
    return (0xffffffff - startTimeMillis + currentTime);
  else
    return(currentTime - startTimeMillis);
}
