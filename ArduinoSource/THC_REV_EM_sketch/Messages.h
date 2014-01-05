/*
 * Messages.h
 *
 *  Created on: Feb 24, 2013
 *      Author: Rege
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "THC.h"

#define VOLTAGE_MASK            0x00
#define COUNTS_BIT 	        0x80
#define TORCH_ON_MASK 		0x40
#define ARC_GOOD_MASK 		0x20
#define VOLT_CTL_ON_MASK 	0x10
#define TORCH_UP_MASK		0x08
#define TORCH_DOWN_MASK		0x04
#define MSB_COUNTS_MASK 	0x03

enum PC_COMMAND
{
	PC_CMD_SYNC = 0,
	PC_CMD_STEP_MODE = 1,
	PC_CMD_GET_MODE = 2,
	PC_CMD_SET_SETPOINT = 3,
	PC_CMD_GET_SETPOINT = 4,
	PC_CMD_DISPLAY_ON_OFF = 5,
	PC_CMD_GET_STATUS = 6,
	PC_CMD_STEP_UNIT = 7,
	PC_CMD_GET_UNIT = 8,
	PC_CMD_GET_CURRENT = 9,
	PC_CMD_TEST_TORCH_ON = 10,
	PC_CMD_TEST_TORCH_OFF = 11,
	PC_CMD_SETPOINT_UP = 12,
	PC_CMD_SETPOINT_DOWN = 13,
        PC_CMD_TEST_ARC_GOOD_ON = 14,
        PC_CMD_TEST_ARC_GOOD_OFF = 15,
        PC_CMD_TEST_THC_UP_ON = 16,
        PC_CMD_TEST_THC_UP_OFF = 17,
        PC_CMD_TEST_THC_DOWN_ON = 18,
        PC_CMD_TEST_THC_DOWN_OFF = 19,
        PC_CMD_CAPTURE_START = 20,
        PC_CMD_CAPTURE_STOP = 21,
        PC_CMD_CAPTURE_GET_STATE = 22,
        PC_SET_COUNT_ON_HYSTERISIS = 23,
        PC_SET_COUNT_OFF_HYSTERISIS = 24,
        PC_SET_TORCH_STABILIZE_MILLISEC = 25
};

enum PC_RESPONSE
{
	PC_RESP_SYNC = 0,
	PC_RESP_MODE = 1,
	PC_RESP_SETPOINT = 2,
	PC_RESP_STATUS = 3,
	PC_RESP_UNIT = 4,
	PC_RESP_CURRENT=5,
	PC_RESP_START_CUT = 6,
	PC_RESP_CAPTURE_OFF = 7,
	PC_RESP_CAPTURE_ON = 8,
	PC_RESP_CUT_PACKET = 9,
	PC_RESP_END_CUT = 10,
	PC_RESP_FAST_VOLT = 11,
};


void SendVoltage(PC_RESPONSE type);
void SendUnits();
void SendMode();
void sendFullVoltageMessage();
void sendVoltageCaptureState(bool captureOnState);
void SendCutEndPacket();
void sendSmallVoltageMessage();


#endif /* MESSAGES_H_ */

