/*
 * Messages.cpp
 *
 *  Created on: Feb 24, 2013
 *      Author: Rege
 */

#include "Arduino.h"
#include "HardwareSerial.h"

#include "THC.h"
#include "Messages.h"
#include "Timer.h"

#include "Platform.h"
#include "IOConfig.h"


Timer cutTimer;

void SendVoltage(PC_RESPONSE type)
{
	uint16_t value;
	uint8_t buff[5];

	if (type == PC_RESP_SETPOINT)
		value = currentStateData.voltSetPoint;
	else
		value = currentStateData.currentVoltage;

	if (currentStateData.currentUnit == THC_UNIT_VOLTS)
		value /= 7;

	// Set the message type.
	buff[0] = type;
	// Load the value
	buff[1] = (uint8_t) ((value >> 8) & MSB_COUNTS_MASK);
	buff[2] = (uint8_t) (value & 0xff);
	// Calculate the checksum
	buff[3] = buff[1] + buff[2];
        buff[4] = 0xff;
	// Send it.
	SERIAL.write(buff, 5);
}

void SendUnits()
{
	uint8_t buff[4];

	// Build and send the response.
	buff[0] = (uint8_t) PC_RESP_UNIT;
	buff[1] = (uint8_t) currentStateData.currentUnit;
        buff[2] = buff[1];
        buff[3] = 0xff;
	SERIAL.write(buff, 4);
}

void SendMode()
{
	uint8_t buff[4];

	// Build and send the response.
	buff[0] = (uint8_t) PC_RESP_MODE;
	buff[1] = (uint8_t) currentStateData.currentMode;
        if (currentStateData.currentMode == THC_MODE_OPERATING)
          buff[1] = buff[1] | ( ((uint8_t) currentStateData.currentState) << 4);
	buff[2] = buff[1];
        buff[3] = 0xff;
	SERIAL.write(buff, 4);
}

#ifdef NOPE
void sendSmallVoltageMessage()
{
	uint8_t byte[7];
	uint16_t millisecondsElapsed;
	uint16_t temp;

	byte[0] = PC_RESP_FAST_VOLT;
	millisecondsElapsed = cutTimer.elapsedMilliSeconds();
	byte[1] = (uint8_t)(millisecondsElapsed >> 8);
	byte[2] = (uint8_t)(millisecondsElapsed & 0xff);

	if (currentStateData.currentUnit == THC_UNIT_VOLTS)
	{
  	  temp = currentStateData.currentVoltage / 7;
          byte[3] = 0;
        }
        else
        {
  	  temp = currentStateData.currentVoltage;
	  byte[3] = (uint8_t) ((temp >> 8) & MSB_COUNTS_MASK);
  	  byte[3] |= COUNT_MASK;
        }

	if (currentStateData.torchOnState)
		byte[3] |= TORCH_ON_MASK;

	if (currentStateData.arcGoodState)
		byte[3] |= ARC_GOOD_MASK;

	if (currentStateData.runVoltageControl)
		byte[3] |= VOLT_CTL_ON_MASK;

	if (currentStateData.torchUp)
		byte[3] |= TORCH_UP_MASK;

	if (currentStateData.torchDown)
		byte[3] |= TORCH_DOWN_MASK;

	byte[4] = (uint8_t) (temp & 0xff);

	byte[5] = byte[1]+byte[2]+byte[3]+byte[4];
        byte[6] = 0xff;
	SERIAL.write((const uint8_t*) byte, 7);
}
#endif

void sendFullVoltageMessage()
	{
	uint8_t byte[9];
	uint16_t millisecondsElapsed;
uint16_t raw;

	byte[0] = PC_RESP_CUT_PACKET;
	millisecondsElapsed = cutTimer.elapsedMilliSeconds();
	byte[1] = (uint8_t)(millisecondsElapsed >> 8);
	byte[2] = (uint8_t)(millisecondsElapsed & 0xff);
	if (currentStateData.currentUnit == THC_UNIT_VOLTS)
	{
                byte[3] = 0;
  	        byte[4] = (uint8_t)((currentStateData.voltSetPoint / 7));
	}
	else
	{
                byte[3] = (uint8_t) ((currentStateData.voltSetPoint >> 8) & MSB_COUNTS_MASK);
        	byte[4] = (uint8_t)(currentStateData.voltSetPoint & 0xff);
	}

      byte[3] |= ((uint8_t) currentStateData.currentMode) << 3;
	// Set the voltage variable and initialize the flag byte
	// based on voltage unit.
	if (currentStateData.currentUnit == THC_UNIT_VOLTS)
	{
		byte[5] = 0;
  	        byte[6] = (uint8_t) (currentStateData.currentVoltage / 7);
	}
	else
	{
		byte[5] = (uint8_t) ((currentStateData.currentVoltage >> 8 ) & MSB_COUNTS_MASK);
  	        byte[6] = (uint8_t) (currentStateData.currentVoltage & 0xff);

	}

	if (currentStateData.torchOnState)
		byte[5] |= TORCH_ON_MASK;

	if (currentStateData.arcGoodState)
		byte[5] |= ARC_GOOD_MASK;

	if (currentStateData.runVoltageControl)
		byte[5] |= VOLT_CTL_ON_MASK;

	if (currentStateData.torchUp)
		byte[5] |= TORCH_UP_MASK;

	if (currentStateData.torchDown)
		byte[5] |= TORCH_DOWN_MASK;

	byte[7] = byte[1]+byte[2]+byte[3]+byte[4]+byte[5]+byte[6];
        byte[8] = 0xff;
	SERIAL.write((const uint8_t*) byte, 9);
	}

void SendCutStartPacket()
	{
  	uint8_t byte[3];

  	cutTimer.startTimer();
        byte[0] = PC_RESP_START_CUT;
        // Checksum on single byte packet is value itself.
        byte[1] = PC_RESP_START_CUT;
        byte[2] = 0xff;
	SERIAL.write((const uint8_t*) byte, 3);
	}

void SendCutEndPacket()
	{
  	uint8_t byte[3];

	cutTimer.startTimer();
        byte[0] = PC_RESP_END_CUT;
        // Checksum on single byte packet is value itself.
        byte[1] = PC_RESP_END_CUT;
        byte[2] = 0xff;
	SERIAL.write((const uint8_t*) byte, 3);
	}


void sendVoltageCaptureState(bool captureOnState)
	{
  	uint8_t byte[3];

	if (captureOnState)
		byte[0] = PC_RESP_CAPTURE_ON;
	else
		byte[0] = PC_RESP_CAPTURE_OFF;

        // Checksum on single byte packet is value itself.
        byte[1] = byte[0];
        byte[2] = 0xff;
	SERIAL.write((const uint8_t*) byte, 3);
	}



