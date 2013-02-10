/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * Isr.hpp
 *
 *  Created on: Sep 22, 2012
 *      Author: Rege
 */

#ifndef ISR_HPP_
#define ISR_HPP_


void torchOnIsr(void);
void arcGoodIsr(void);
void setupIO(void);

unsigned int decodeLogicLevel(int currentLevel, bool activeHigh);

#endif /* ISR_HPP_ */
