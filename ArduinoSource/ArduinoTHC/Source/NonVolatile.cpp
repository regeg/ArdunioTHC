/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * NonVolatile.cpp
 *
 *  Created on: Sep 22, 2012
 *      Author: Rege
 */

#include "Arduino.h"
#include "EEPROM.h"

#include "NonVolatile.hpp"
#include "THC.h"

//
// NonVolatileStorage Constructor
//
// When created, just set the flag to denote that no data is loaded.
//
NonVolatileStorage::NonVolatileStorage()
	{
	initialized = false;

	// None of the following need initialized, but do
	// it anyway to prevent uninitialized variable warnings.
	addrOffset = 1;
	scale = DEFAULT_SCALE;
	voltSetPoint = DEFAULT_SET_POINT;
	pilotArcCount = 0;
	pierceCount = 0;
	tipHours = 0;
	tipMinutes = 0;
	tipSeconds = 0;
	writeCount = 0;
	defaultState = THC_STATE_ENABLED;

	}

//
// InitializeStorage
//
// This call if used to reset all values and then write
// those reset values to the flash.  It is typically used when
// the flash is blank or if the flash data is corrupt (determined
// based on a magic number).
//
//
void NonVolatileStorage::InitializeStorage(unsigned char addrOffsetParam)
	{
	unsigned int addrOffsetValue;

	// Initialize all the EEPROM memory assuming this
	// is a new system.
	addrOffset = addrOffsetParam;
	scale = DEFAULT_SCALE;
	voltSetPoint = DEFAULT_SET_POINT;
	pilotArcCount = 0;
	pierceCount = 0;
	tipHours = 0;
	tipMinutes = 0;
	tipSeconds = 0;
	writeCount = 0;
	defaultState = THC_STATE_ENABLED;

	addrOffsetValue = (addrOffset * MEMORY_BLOCK_SIZE) + 1;

	//
	// Now save the initialized data.
	//

	// Save the offset value
	EEPROM.write(ADDR_OFFSET, addrOffset);

	// Save the values that had defaults.
	EEPROM.write(REL_ADDR_VOLT_SCALING + addrOffsetValue, scale);
	EEPROM.write(REL_ADDR_VOLT_SETPOINT_B1 + addrOffsetValue, (unsigned char) (voltSetPoint & 0xff));
	EEPROM.write(REL_ADDR_VOLT_SETPOINT_B2 + addrOffsetValue, (unsigned char) (voltSetPoint & 0xff00) >> 8);
	EEPROM.write(REL_ADDR_DEFAULT_STATE + addrOffsetValue,
			(unsigned char) defaultState);

	// Save all the values that are just zeroed.
	EEPROM.write(REL_ADDR_CHECK_BYTE + addrOffsetValue, EEPROM_MAGIC_NUMBER);
	EEPROM.write(REL_ADDR_PILOT_ARC_COUNT_BYTE1 + addrOffsetValue, 0);
	EEPROM.write(REL_ADDR_PILOT_ARC_COUNT_BYTE2 + addrOffsetValue, 0);
	EEPROM.write(REL_ADDR_PIERCE_COUNT_BYTE1 + addrOffsetValue, 0);
	EEPROM.write(REL_ADDR_PIERCE_COUNT_BYTE2 + addrOffsetValue, 0);
	EEPROM.write(REL_ADDR_TIP_HOURS_BYTE + addrOffsetValue, 0);
	EEPROM.write(REL_ADDR_TIP_MINUTES_BYTE + addrOffsetValue, 0);
	EEPROM.write(REL_ADDR_TIP_SECONDS_BYTE + addrOffsetValue, 0);
	EEPROM.write(REL_ADDR_WRITE_COUNT_BYTE1 + addrOffsetValue, 0);
	EEPROM.write(REL_ADDR_WRITE_COUNT_BYTE2 + addrOffsetValue, 0);
	EEPROM.write(REL_ADDR_WRITE_COUNT_BYTE3 + addrOffsetValue, 0);
	EEPROM.write(REL_ADDR_WRITE_COUNT_BYTE4 + addrOffsetValue, 0);
	}

//
// DataLoad
//
// This method will load all the data from EEPROM.  If there
// is no valid data (determined by an invalid check byte),
// then it will initialize everything and save it.
//
void NonVolatileStorage::DataLoad(void)
	{
	unsigned char byte1, byte2;
	unsigned int addrOffsetValue;

	// Read the offset value to see where the data is in RAM
	addrOffset = EEPROM.read(ADDR_OFFSET);
	addrOffsetValue = (addrOffset * MEMORY_BLOCK_SIZE) + 1;
	// Get the magic number.
	byte1 = EEPROM.read(REL_ADDR_CHECK_BYTE + addrOffsetValue);

	// If the magic number is not right, assume the
	// EEPROM has not been initialized.
	if (byte1 != EEPROM_MAGIC_NUMBER)
		{
		DebugPrint("No NVRam Data\n");
		InitializeStorage(0);
		return;
		}

	//
	// Read each of the values and load them into their variables.
	//
	DebugPrint("\nNonVolatileStorage::DataLoad()\n");

	DebugPrint("\nMemory Block: ");
	DebugPrint(addrOffsetValue);

	voltSetPoint = EEPROM.read(REL_ADDR_VOLT_SETPOINT_B1 + addrOffsetValue);
	voltSetPoint |= (EEPROM.read(REL_ADDR_VOLT_SETPOINT_B2 + addrOffsetValue) << 8);
	DebugPrint("\nvoltSetPoint: ");
	DebugPrint(voltSetPoint);

	scale = EEPROM.read(REL_ADDR_VOLT_SCALING + addrOffsetValue);
	DebugPrint("\nScale: ");
	DebugPrint(scale);

	byte1 = EEPROM.read(REL_ADDR_PILOT_ARC_COUNT_BYTE1 + addrOffsetValue);
	byte2 = EEPROM.read(REL_ADDR_PILOT_ARC_COUNT_BYTE2 + addrOffsetValue);
	pilotArcCount = (((unsigned int) byte1) << 8) | ((unsigned int) byte2);
	DebugPrint("\nPilot Arc Count: ");
	DebugPrint(pilotArcCount);

	byte1 = EEPROM.read(REL_ADDR_PIERCE_COUNT_BYTE1 + addrOffsetValue);
	byte2 = EEPROM.read(REL_ADDR_PIERCE_COUNT_BYTE2 + addrOffsetValue);
	pierceCount = (((unsigned int) byte1) << 8) | ((unsigned int) byte2);
	DebugPrint("\nPierce Count: ");
	DebugPrint(pierceCount);

	tipHours = EEPROM.read(REL_ADDR_TIP_HOURS_BYTE + addrOffsetValue);
	DebugPrint("\nTip Hours: ");
	DebugPrint(tipHours);
	tipMinutes = EEPROM.read(REL_ADDR_TIP_MINUTES_BYTE + addrOffsetValue);
	DebugPrint("\nTip Minutes: ");
	DebugPrint(tipMinutes);
	tipSeconds = EEPROM.read(REL_ADDR_TIP_SECONDS_BYTE + addrOffsetValue);
	DebugPrint("\nTip Seconds: ");
	DebugPrint(tipSeconds);

	defaultState = (THCState_e) EEPROM.read(REL_ADDR_DEFAULT_STATE);

	writeCount = 0;
	writeCount = ((unsigned long) EEPROM.read(
			REL_ADDR_WRITE_COUNT_BYTE1 + addrOffsetValue)) << 24;
	writeCount |= ((unsigned long) EEPROM.read(
			REL_ADDR_WRITE_COUNT_BYTE2 + addrOffsetValue)) << 16;
	writeCount |= ((unsigned long) EEPROM.read(
			REL_ADDR_WRITE_COUNT_BYTE3 + addrOffsetValue)) << 8;
	writeCount |= ((unsigned long) EEPROM.read(
			REL_ADDR_WRITE_COUNT_BYTE4 + addrOffsetValue));
	DebugPrint("\nWrite Count: ");
	DebugPrint(writeCount);
	DebugPrint("\n\n");

	initialized = true;
	}

//
// DataSave
//
// Save the current object values in EEPROM.  If not initialized
// properly, then just return.
//
void NonVolatileStorage::DataSave(void)
	{
	unsigned int addrOffsetValue;

	// Don't write anything if the values were never initialized.
	if (!initialized)
		return;
	addrOffsetValue = (addrOffset * MEMORY_BLOCK_SIZE) + 1;

	// Increment the count of data saved.
	writeCount++;
	// If we've exceeded the EEPROM write limit, move to the next
	// block of memory.
	if (writeCount >= WRITE_LIMIT)
		addrOffset += MEMORY_BLOCK_SIZE;

	// Now, write all the values.
	EEPROM.write(REL_ADDR_PILOT_ARC_COUNT_BYTE1 + addrOffsetValue,
			(unsigned char) pilotArcCount >> 8);
	EEPROM.write(REL_ADDR_PILOT_ARC_COUNT_BYTE2 + addrOffsetValue,
			(unsigned char) pilotArcCount & 0xff);
	EEPROM.write(REL_ADDR_PIERCE_COUNT_BYTE1 + addrOffsetValue,
			(unsigned char) (pierceCount >> 8));
	EEPROM.write(REL_ADDR_PIERCE_COUNT_BYTE2 + addrOffsetValue,
			(unsigned char) (pierceCount & 0xff));
	EEPROM.write(REL_ADDR_TIP_HOURS_BYTE + addrOffsetValue, tipHours);
	EEPROM.write(REL_ADDR_TIP_MINUTES_BYTE + addrOffsetValue, tipMinutes);
	EEPROM.write(REL_ADDR_TIP_SECONDS_BYTE + addrOffsetValue, tipSeconds);
	EEPROM.write(REL_ADDR_VOLT_SCALING + addrOffsetValue, scale);
	EEPROM.write(REL_ADDR_WRITE_COUNT_BYTE1 + addrOffsetValue,
			(unsigned char) (writeCount >> 24));
	EEPROM.write(REL_ADDR_WRITE_COUNT_BYTE2 + addrOffsetValue,
			(unsigned char) (writeCount >> 16) & 0xff);
	EEPROM.write(REL_ADDR_WRITE_COUNT_BYTE3 + addrOffsetValue,
			(unsigned char) (writeCount >> 8) & 0xff);
	EEPROM.write(REL_ADDR_WRITE_COUNT_BYTE4 + addrOffsetValue,
			(unsigned char) writeCount & 0xff);
	EEPROM.write(REL_ADDR_VOLT_SETPOINT_B1 + addrOffsetValue,
			(unsigned char) (voltSetPoint & 0xff));
	EEPROM.write(REL_ADDR_VOLT_SETPOINT_B2 + addrOffsetValue,
			(unsigned char) ((voltSetPoint & 0xff00) >> 8));
	EEPROM.write(REL_ADDR_DEFAULT_STATE + addrOffsetValue,
			(unsigned char) defaultState);
	}

//
// DataGetAll()
//
// Convenience function that will load all the saved variables
// at one time.
//
// Parameters
//	In/Out - data structure updated with all values.
//
void NonVolatileStorage::DataGetAll(THCStatData* data)
	{
	if (!initialized)
		DataLoad();

	data->voltageScaleFactor = scale;
	data->pilotArcCount = pilotArcCount;
	data->pierceCount = pierceCount;
	data->tipHours = tipHours;
	data->tipMinutes = tipMinutes;
	data->tipSeconds = tipSeconds;
	data->voltSetPoint = voltSetPoint;
	data->defaultState = (THCState_e) defaultState;
	data->initialWriteCount = writeCount;
	data->flashBlock = addrOffset;
	}

//
// DataSetAll
//
// Set all object properties based on the structure pointer
// passed in.  If not initialized, then just return.
//
// Parameters:
//	In/Out - ptr to data structure to set object values to
//
void NonVolatileStorage::DataSetAll(THCStatData* data)
	{
	if (!initialized)
		return;

	scale = data->voltageScaleFactor;
	pilotArcCount = data->pilotArcCount;
	pierceCount = data->pierceCount;
	tipHours = data->tipHours;
	tipMinutes = data->tipMinutes;
	tipSeconds = data->tipSeconds;
	voltSetPoint = data->voltSetPoint;
	defaultState = (THCState_e) data->defaultState;
	}

//
// ScalingGet()
//
// Return the voltage scaling factor.
//
unsigned char NonVolatileStorage::ScalingGet(void)
	{
	return scale;
	}

//
// ScalingSet()
//
// Set the voltage scaling factor.
//
void NonVolatileStorage::ScalingSet(unsigned char scaleVal)
	{
	scale = scaleVal;
	}

//
// PilotArcCountGet()
//
// Returns the number of times the Pilot Arc was ignited.
//
unsigned int NonVolatileStorage::PilotArcCountGet(void)
	{
	return pilotArcCount;
	}

//
// PilotArcCountSet()
//
// Sets the number of times the Pilot Arc was ignited.
//
void NonVolatileStorage::PilotArcCountSet(unsigned int pilotArcVal)
	{
	pilotArcCount = pilotArcVal;
	}

//
// PeirceCountGet()
//
// Returns the count of pierces performed.
//
unsigned int NonVolatileStorage::PierceCountGet(void)
	{
	return pierceCount;
	}

//
// PeirceCountSet()
//
// Sets the count of pierces performed.
//
void NonVolatileStorage::PierceCountSet(unsigned int count)
	{
	pierceCount = count;
	}

//
// TipHoursGet()
//
// Gets the number of hours of cutting time on the
// current tip.
//
unsigned char NonVolatileStorage::TipHoursGet(void)
	{
	return tipHours;
	}

//
// TipHoursSet()
//
// Gets the number of hours of cutting time on the
// current tip.
//
void NonVolatileStorage::TipHoursSet(unsigned char hours)
	{
	tipHours = hours;
	}

//
// TipMinutesGet()
//
// Gets the number of minutes of cutting time on the
// current tip.
//
unsigned char NonVolatileStorage::TipMinutesGet(void)
	{
	return tipMinutes;
	}

//
// TipMinutesSet()
//
// Sets the number of minutes of cutting time on the
// current tip.
//
void NonVolatileStorage::TipMinutesSet(unsigned char mins)
	{
	tipMinutes = mins;
	}

//
// WriteCountGet()
//
// Returns the count of writes to this block of EEPROM.
//
unsigned char NonVolatileStorage::NonVolatileStorage::WriteCountGet(void)
	{
	return writeCount;
	}

//
// VoltSetPointGet()
//
// Returns the voltage set point for cutting.
//
unsigned int NonVolatileStorage::VoltSetPointGet(void)
	{
	return voltSetPoint;
	}

//
// VoltSetPointSet()
//
// Sets the voltage set point for cutting.
//
void NonVolatileStorage::VoltSetPointSet(unsigned int volts)
	{
	voltSetPoint = volts;
	}

//
// DefaultStateGet()
//
// Returns the default power up state.
//
THCState_e NonVolatileStorage::DefaultStateGet(void)
	{
	return (THCState_e) defaultState;
	}

//
// DefaultStateSet()
//
// Sets the default power up state.
//
void NonVolatileStorage::DefaultStateSet(THCState_e state)
	{
	defaultState = state;
	}


unsigned char NonVolatileStorage::getBlock(void)
	{
	return addrOffset;
	}




unsigned char NonVolatileStorage::setBlock(unsigned char newBlockAddr)
	{
	if (newBlockAddr > 5)
		newBlockAddr = 0;

	if (newBlockAddr != addrOffset)
		{
		// Set the new base block address.
		addrOffset = newBlockAddr;
		// Reset the write count.
		writeCount = 0;
		// Save the offset value
		EEPROM.write(ADDR_OFFSET, addrOffset);
		// Save all the data to the new location.
		DataSave();
		}
	return addrOffset;
	}

void NonVolatileStorage::DataReset(void)
	{
	// Reset the usage counts.
	pilotArcCount = 0;
	pierceCount = 0;
	tipHours = 0;
	tipMinutes = 0;
	// Save the reset values.
	DataSave();
	}
