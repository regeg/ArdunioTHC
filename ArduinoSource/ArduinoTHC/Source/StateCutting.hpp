/*
 * COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * StateCutting.hpp
 *
 *  Created on: Oct 15, 2012
 *      Author: Rege
 */

#ifndef STATECUTTING_HPP_
#define STATECUTTING_HPP_


#include "THCInterface.hpp"

#define VOLTAGE_ON_HYSTERESIS 4 // 7 counts = 1 volt

// Had a great capture at 3, but never got closer then 2, so trying 1
#define VOLTAGE_OFF_HYSTERESIS 1 // 7 counts = 1 volt


#define TORCH_STABILIZE_TIME_MLLISECONDS 1200

Command_e StateCuttingHandler(THCInterface *display);



#endif /* STATECUTTING_HPP_ */
