/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * HardwareConfig.h
 *
 *  Created on: Sep 23, 2012
 *      Author: Rege
 */

#ifndef HARDWARECONFIG_H_
#define HARDWARECONFIG_H_

//
// DIGITAL INPUTS
//

// INPUTS

// INPUTS - FROM USER
// Define the pins for the UI buttons.
#define IN_D_BUTTON_MODE	33
#define IN_D_BUTTON_SELECT 	35
#define IN_D_BUTTON_CANCEL  37
#define IN_D_BUTTON_UP 		39
#define IN_D_BUTTON_DOWN 	41
// Use macros to define active high or low for
// each of the input pins.
//
#define IN_D_BUTTON_SELECT_ACTIVE	LOW
#define IN_D_BUTTON_CANCEL_ACTIVE 	LOW
#define IN_D_BUTTON_UP_ACTIVE 		LOW
#define IN_D_BUTTON_DOWN_ACTIVE 	LOW
#define IN_D_BUTTON_MODE_ACTIVE 	LOW

// INPUTS - FROM CNC
// Define the pin for the torch on request
// from the CNC
#define IN_D_INT_TORCH_ON_PIN 	21
#define VECTOR_TORCH_ON_ISR	2

// iNPUTS - FROM PLASMA
// Define the pin for the arc okay signal
// from the plasma
#define IN_D_INT_ARC_GOOD_PIN 	20
#define VECTOR_ARC_GOOD_ISR	3

// OUTPUTS


// OUTPUTS - TO CNC
#define OUT_D_ARC_GOOD 		50
#define OUT_D_TORCH_UP 		48
#define OUT_D_TORCH_DOWN	46

// OUTPUTS - TO PLASMA
// Output to trip relay to turn on torch
#define OUT_D_TORCH_RELAY	17

//
// Define what active high and active low mean.
//#define ACTIVE_LOW		0
//define ACTIVE_HIGH		1


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
#define OUT_D_TORCH_RELAY_NOT_ACTIVE	LOW


//
// ANALOG I/O
//

// Define the analog in for torch voltage sensing.
#define IN_A_VOLTAGE (8)



inline unsigned decodeLogicLevel(unsigned currentLevel, unsigned activeLevel);


#endif /* HARDWARECONFIG_H_ */
