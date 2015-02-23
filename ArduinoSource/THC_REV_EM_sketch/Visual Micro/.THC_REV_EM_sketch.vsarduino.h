#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
//Board = Arduino Mega 2560 or Mega ADK
#define __AVR_ATmega2560__
#define 
#define ARDUINO 105
#define ARDUINO_MAIN
#define __AVR__
#define F_CPU 16000000L
#define __cplusplus
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

void AddSecondsToTipUse(THCStatData *usageData, unsigned int seconds);
//
void handleModeChange();
//
void TestTorchOn(bool value);
void processCommand(Command_e newCmd);

#include "c:\arduino-1.0.5\hardware\arduino\variants\mega\pins_arduino.h" 
#include "c:\arduino-1.0.5\hardware\arduino\cores\arduino\arduino.h"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\THC_REV_EM_sketch.ino"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\BypassMode.cpp"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\BypassMode.h"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\IO.cpp"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\IO.h"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\IOConfig.h"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\Messages.cpp"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\Messages.h"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\NonVolatile.cpp"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\NonVolatile.h"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\Platform.h"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\StateCutting.cpp"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\StateCutting.h"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\StateEnabled.cpp"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\StateEnabled.h"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\THC.h"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\Timer.cpp"
#include "c:\github-repo\ArduinoTHC\ArduinoSource\THC_REV_EM_sketch\Timer.h"
#endif
