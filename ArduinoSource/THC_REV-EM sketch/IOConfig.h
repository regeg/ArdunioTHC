/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * IOConfig.h
 *
 *  Created on: Sep 23, 2012
 *      Author: Rege
 */

#ifndef IO_CONFIG_H_
#define IO_CONFIG_H_

#include "Platform.h"

//
// DIGITAL INPUTS
//

// INPUTS - FROM CNC
// Define the pin for the torch on request
// from the CNC
#ifdef MICRO_THC
#define IN_D_TORCH_ON_PIN      6
#else
#define IN_D_TORCH_ON_PIN      10
#endif

// INPUTS - FROM PLASMA
// Define the pin for the arc okay signal
// from the plasma
#ifdef MICRO_THC
#define IN_D_ARC_GOOD_PIN 	8
#else
#define IN_D_ARC_GOOD_PIN 	12
#endif

//
// DIGITAL OUTPUTS
//

// OUTPUTS - TO CNC
#ifdef MICRO_THC
#define OUT_D_ARC_GOOD 		3
#define OUT_D_TORCH_UP 		2
#define OUT_D_TORCH_DOWN	4
#else
#define OUT_D_ARC_GOOD 		2
#define OUT_D_TORCH_UP 		3
#define OUT_D_TORCH_DOWN	4
#endif

// OUTPUTS - TO PLASMA
// Output to trip relay to turn on torch
#ifdef MICRO_THC
#define OUT_D_TORCH_RELAY	7
#else
#define OUT_D_TORCH_RELAY	11
#endif


#define IN_D_TORCH_ON_PIN_ACTIVE 	LOW
#define IN_D_ARC_GOOD_PIN_ACTIVE 	LOW

//
// Use macros to define active high or low for
// each of the output pins.
//
#define OUT_D_ARC_GOOD_ACTIVE			LOW
#define OUT_D_ARC_GOOD_NOT_ACTIVE		HIGH
#define OUT_D_TORCH_UP_ACTIVE			LOW
#define OUT_D_TORCH_UP_NOT_ACTIVE		HIGH
#define OUT_D_TORCH_DOWN_ACTIVE			LOW
#define OUT_D_TORCH_DOWN_NOT_ACTIVE		HIGH
#define OUT_D_TORCH_RELAY_ACTIVE		HIGH
#define OUT_D_TORCH_RELAY_NOT_ACTIVE	        LOW

//
// ANALOG I/O
//

// Define the analog in for torch voltage sensing.
#ifdef MICRO_THC
#define IN_A_VOLTAGE A1
#else
#define IN_A_VOLTAGE A0
#endif
#endif /* IO_CONFIG_H_ */

