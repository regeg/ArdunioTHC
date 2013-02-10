/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * Timer.h
 *
 * This is a simple timer class that will provide elapsed seconds or
 * milliseconds from a given start time.
 *
 *  Created on: Sep 3, 2012
 *      Author: Rege George
 */
#ifndef TIMER_H_
#define TIMER_H_



class Timer
    {
private:
    unsigned long startTimeMillis;  // Start time from the Ardunio millis() call.

public:

    Timer();

    // Use this function to either start or restart the time
    void startTimer();

    // Return the number of elapsed seconds.
    unsigned long elapsedSeconds();

    // Return the number of elapsed millisconds.
    unsigned long elapsedMilliSeconds();

    };

#endif /* TIMER_H_ */
