/*
 * Messages.cpp
 *
 *  Created on: Feb 24, 2013
 *      Author: Rege
 */

#include "Arduino.h"
#include "HardwareSerial.h"

#include "Messages.h"
#include "Timer.hpp"

Timer cutTimer;

void SendVoltage(PC_RESPONSE type)
{
	uint16_t value;
	uint8_t buff[3];

	if (type == PC_RESP_SETPOINT)
		value = currentStateData.voltSetPoint;
	else
		value = currentStateData.currentVoltage;

	if (currentStateData.currentUnit == THC_UNIT_VOLTS)
		value /= 7;

	buff[0] = type;
	buff[1] = (uint8_t) (value >> 8);
	buff[2] = (uint8_t) (value & 0xff);
	Serial1.write(buff, 3);
}

void SendUnits()
{
	uint8_t buff[2];

	// Build and send the response.
	buff[0] = (uint8_t) PC_RESP_UNIT;
	buff[1] = (uint8_t) currentStateData.currentUnit;
	Serial1.write(buff, 2);
}

void SendMode()
{
	uint8_t buff[2];

	// Build and send the response.
	buff[0] = (uint8_t) PC_RESP_MODE;
	buff[1] = (uint8_t) currentStateData.currentMode;
	Serial1.write(buff, 2);
}


void sendCutVoltageMessage()
	{
	uint8_t byte[5];
	uint16_t millisecondsElapsed;

	byte[0] = PC_RESP_CUT_PACKET;
	millisecondsElapsed = cutTimer.elapsedMilliSeconds();
	byte[1] = (uint8_t)(millisecondsElapsed >> 8);
	byte[2] = (uint8_t)(millisecondsElapsed & 0xff);
	byte[3] = COUNT_MASK;

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

	byte[3] |= (char) ((currentStateData.currentVoltage >> 8) & MSB_COUNTS_MASK);

	byte[4] = (uint8_t) (currentStateData.currentVoltage & 0xff);

	Serial1.write((const uint8_t*) byte, 5);
	}

void SendCutStartPacket()
	{
		cutTimer.startTimer();
		Serial1.write(PC_RESP_START_CUT);
	}

void SendCutEndPacket()
	{
		cutTimer.startTimer();
		Serial1.write(PC_RESP_END_CUT);
	}

void sendVoltageCaptureState(bool captureOnState)
	{

	if (captureOnState)
		Serial1.write(PC_RESP_CAPTURE_ON);
	else
		Serial1.write(PC_RESP_CAPTURE_OFF);
	}


