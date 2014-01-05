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

// Was 4 but changed it because noise is +/- 2
#define VOLTAGE_ON_HYSTERESIS 5 // 7 counts = 1 volt

// Had a great capture at 3, but never got closer then 2, so trying 1
#define VOLTAGE_OFF_HYSTERESIS 2 // 7 counts = 1 volt


#define TORCH_STABILIZE_TIME_MLLISECONDS 0

void StateCuttingHandler();



#endif /* STATECUTTING_HPP_ */

