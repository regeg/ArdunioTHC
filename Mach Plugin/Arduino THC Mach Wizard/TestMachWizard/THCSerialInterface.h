#pragma once


using namespace System;
using namespace System::Threading;
using namespace System::IO::Ports;

//#include "ComError.h"

namespace ArduinoTHC {}				// setup default namespace for .NET

#pragma managed



#define LED_TORCH_ON		2001
#define LED_ARC_GOOD		2002
#define LED_CONTROL_ACTIVE	2003
#define LED_TORCH_UP		2004
#define LED_TORCH_DOWN		2005
#define LED_UNITS_COUNTS	2006
#define LED_UNITS_VOLTS		2007
#define	LED_THC_STATUS		2008
#define LED_MODE_DISABLED	2009
#define LED_MODE_BYPASS		2010
#define LED_MODE_CRUISE		2011
#define LED_MODE_OPERATING	2012

#define DRO_SET_POINT		1101
#define DRO_CURRENT			1102
#define DRO_CHECKSUM		1103

#define BUTTON_SETPOINT_UP		10101
#define BUTTON_SETPOINT_DOWN	10102
#define BUTTON_TOGGLE_UNITS		10103
#define BUTTON_TOGGLE_MODE		10104

#define VOLTAGE_MASK        0x00
#define COUNT_MASK 	        0x80
#define TORCH_ON_MASK 		0x40
#define ARC_GOOD_MASK 		0x20
#define VOLT_CTL_ON_MASK 	0x10
#define TORCH_UP_MASK		0x08
#define TORCH_DOWN_MASK		0x04
#define MSB_COUNTS_MASK 	0x03

DWORD WINAPI SerialProcessing(LPVOID lpParam);


void ThcSetSetPoint(UInt16 value);

