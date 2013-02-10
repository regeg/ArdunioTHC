/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * NonVolatile.hpp
 *
 * The NonVolatile class will maintain the THC settings and usage
 * data in the EEPROM on the Arduino.  It uses a checksum byte
 * to determine if the memory is blank or corrupt.  If blank
 * or corrupt, it will re-initialize the flahs.
 * Since the EEPROM has a limited number of writes, the write
 * count is tracked and if the write limit is exceeded, then
 * the block of memory used is moved up in EEPROM.
 *
 *  Created on: Sep 22, 2012
 *      Author: Rege
 */
#ifndef NONVOLATILE_HPP_
#define NONVOLATILE_HPP_

#include "THC.h"

// Define the memory map of data saved in EEPROM.
#define ADDR_OFFSET			0  // The offset is the offset to the memory block holding the data.
					   // This is ALWAYS at the absolute 0 address in the flash.

// The following bytes are relative offsets within the block of memory
#define REL_ADDR_CHECK_BYTE 		 0
#define REL_ADDR_VOLT_SETPOINT_B1 	1
#define REL_ADDR_VOLT_SETPOINT_B2 	2
#define REL_ADDR_VOLT_SCALING		3
#define REL_ADDR_PILOT_ARC_COUNT_BYTE1 	4
#define REL_ADDR_PILOT_ARC_COUNT_BYTE2 	5
#define REL_ADDR_PIERCE_COUNT_BYTE1    	6
#define REL_ADDR_PIERCE_COUNT_BYTE2    	7
#define REL_ADDR_TIP_HOURS_BYTE			8
#define REL_ADDR_TIP_MINUTES_BYTE		9
#define REL_ADDR_TIP_SECONDS_BYTE		10
#define REL_ADDR_WRITE_COUNT_BYTE1		11
#define REL_ADDR_WRITE_COUNT_BYTE2		12
#define REL_ADDR_WRITE_COUNT_BYTE3		13
#define REL_ADDR_WRITE_COUNT_BYTE4		14
#define REL_ADDR_DEFAULT_STATE			15

#define MEMORY_BLOCK_SIZE				16

// Define default values for a new system.
#define DEFAULT_SCALE 		40
#define DEFAULT_SET_POINT 	600


// The magic number is used to see if the EEPROM has
// been initialized.
#define EEPROM_MAGIC_NUMBER 42


// Set the max number of writes before using
// the next sequential block of EEPROM
#define WRITE_LIMIT 95000L



//
// This class handles all persistent data kept in EEPROM.
//
class NonVolatileStorage {

private:
	bool initialized;
	unsigned char addrOffset;
	unsigned char scale;
	unsigned int  pilotArcCount;
	unsigned int  pierceCount;
	unsigned char tipHours;
	unsigned char tipMinutes;
	unsigned char tipSeconds;
	unsigned int voltSetPoint;
	unsigned long writeCount;
	THCState_e defaultState;

	void InitializeStorage(unsigned char addrOffsetParam);

public:
	NonVolatileStorage();

	void DataLoad(void);
	void DataSave(void);
	void DataReset(void);

	void DataGetAll(THCStatData *data);
	void DataSetAll(THCStatData *data);

	unsigned char ScalingGet(void);
	void ScalingSet(unsigned char);

	unsigned int PilotArcCountGet(void);
	void PilotArcCountSet(unsigned int);

	unsigned int PierceCountGet(void);
	void PierceCountSet(unsigned int);

	unsigned char TipHoursGet(void);
	void TipHoursSet(unsigned char);

	unsigned char TipMinutesGet(void);
	void TipMinutesSet(unsigned char);

	unsigned char WriteCountGet(void);

	unsigned int VoltSetPointGet(void);
	void VoltSetPointSet(unsigned int);

	THCState_e DefaultStateGet(void);
	void DefaultStateSet(THCState_e state);

	unsigned char getBlock(void);
	unsigned char setBlock(unsigned char newBlock);

};


#endif /* NONVOLATILE_HPP_ */
