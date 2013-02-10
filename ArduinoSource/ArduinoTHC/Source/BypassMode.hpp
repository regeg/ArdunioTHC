/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * BypassMode.hpp
 *
 *  Created on: Dec 28, 2012
 *      Author: Rege
 */

#ifndef BYPASSMODE_HPP_
#define BYPASSMODE_HPP_

#include "THCInterface.hpp"

class BypassMode {

private:
		bool torchOn;
		bool arcGood;
		THCInterface *display;

		BypassMode();

public:
		BypassMode(THCInterface *displayPtr);

		void controlLoop();
};


#endif /* BYPASSMODE_HPP_ */
