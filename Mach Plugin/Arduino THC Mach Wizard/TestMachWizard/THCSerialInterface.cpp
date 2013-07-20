#include "stdafx.h"
#include "Plugin.h"
#include "MachDevice.h"
#include "ConfigDialog.h"
#include "PlugInControlDialog.h"
#include "XYZDemoDialog.h"
#include "XMLNetProfile.h"
#include "THCSerialInterface.h"
#include "ManagedGlobal.h"

#include <stdlib.h>

//---------------------------------------------------------------------
//	the default namespace for managed functions and classes
//---------------------------------------------------------------------

// Regis M George
using namespace ArduinoTHC;


void ThcSetSetPoint(UInt16 value)
{
	array<unsigned char>^ cmd = gcnew array<unsigned char>(3);

	cmd[0] = (unsigned char)THC_CMD_SET_SETPOINT;
    cmd[1] = (unsigned char)(value >> 8);
    cmd[2] = (unsigned char)(value & 0xff);
	MG::thcSerial->Write(cmd, 0, 3);
}

void RequestCurrentMode()
{
	array<unsigned char>^ buffer = gcnew array<unsigned char>(8);

	buffer[0] = (unsigned char) THC_CMD_GET_MODE;
	MG::thcSerial->Write(buffer, 0, 1);
}

#ifdef NOPE
//
// byte1, byte2 - time in milliseconds
// byte3 - flags, upper nibble of current voltage (if in counts)
// byte4 - lower byte of voltage (counts or volts)
// byte 5 - checksum
void decodeFastVoltageOrCounts(array<unsigned char>^ buffer)
{
    unsigned char checksum;
	UInt16	elapsedTime;

    checksum = (unsigned char)(buffer[1] + buffer[2] + buffer[3] + buffer[4]);

    if (checksum != buffer[5])
		MG::thcData.checksumErrorCnt++;

    elapsedTime = (UInt16)(((UInt16) buffer[1]) << 8);
    elapsedTime |= (UInt16) buffer[2];

    // Bit set to 1 if A/D count
	MG::thcData.torchOn = ((buffer[3] & TORCH_ON_MASK) != 0) ? true : false;

	MG::thcData.arcGood = ((buffer[3] & ARC_GOOD_MASK) != 0) ? true : false;
	MG::thcData.voltControlOn = ((buffer[3] & VOLT_CTL_ON_MASK) != 0) ? true : false;
	MG::thcData.torchUp = ((buffer[3] & TORCH_UP_MASK) != 0) ? true : false;
    MG::thcData.torchDown = ((buffer[3] & TORCH_DOWN_MASK) != 0) ? true : false;

    MG::thcData.currentVoltage = (UInt16) buffer[4];
    MG::thcData.currentVoltage += ( (UInt16) (MSB_COUNTS_MASK & buffer[3])) << 8;
}
#endif






void decodeFullVoltageMessage(array<unsigned char>^ buffer)
{
    unsigned char checksum;
	UInt16	elapsedTime;
//	THC_UNIT msgUnit;

    checksum = (unsigned char)(buffer[1] + buffer[2] + buffer[3] + buffer[4]+ buffer[5]+ buffer[6]);

    if (checksum != buffer[7])
	{
		MG::thcData.checksumErrorCnt++;
		return;
	}

	// Decode the timestamp (in milliseconds).
    elapsedTime = ((UInt16)buffer[1]);
    elapsedTime = ((UInt16) buffer[2]) + (elapsedTime << 8);

	// Encode the current setpoint.
    MG::thcData.currentSetPoint = (UInt16)(MSB_COUNTS_MASK & buffer[3]);
    MG::thcData.currentSetPoint = (UInt16) buffer[4] + (MG::thcData.currentSetPoint << 8);
	
	MG::thcData.currentMode = (THC_MODE) (buffer[3] >> 3);

    MG::thcData.currentVoltage = (UInt16)(MSB_COUNTS_MASK & buffer[5]);
    MG::thcData.currentVoltage = (UInt16) buffer[6] + (MG::thcData.currentVoltage << 8);

	// Bit set to 1 if A/D count
	//MG::thcData.currentUnit = ((buffer[6] & COUNT_MASK) != COUNT_MASK) ? THC_UNIT_COUNT : THC_UNIT_VOLT;

	MG::thcData.torchOn = ((buffer[5] & TORCH_ON_MASK) != 0) ? true : false;
	MG::thcData.arcGood = ((buffer[5] & ARC_GOOD_MASK) != 0) ? true : false;
	MG::thcData.voltControlOn = ((buffer[5] & VOLT_CTL_ON_MASK) != 0) ? true : false;
	MG::thcData.torchUp = ((buffer[5] & TORCH_UP_MASK) != 0) ? true : false;
    MG::thcData.torchDown = ((buffer[5] & TORCH_DOWN_MASK) != 0) ? true : false;

}




void ReadSomeBytes(array<unsigned char>^ buffer, UInt16 offset, UInt16 count)
{
	while ((MG::thcSerial->BytesToRead < count))
	{
		// Kill the thread if requested.
		if (MG::thcControl.hariKari)
			ExitThread(0);
		// Didn't get killed, so sleep a little
		Sleep(5);
	}

	// Read the bytes.
	MG::thcSerial->Read(buffer, offset, count);
}


DWORD WINAPI SerialProcessing(LPVOID lpParam)
{
    unsigned char checksum;
    UInt16 value;
	THCInterfaceControl localControl;
	array<unsigned char>^ buffer = gcnew array<unsigned char>(8);

	MG::thcSerial = gcnew SerialPort();
	MG::thcSerial->BaudRate = 115200;
	MG::thcSerial->DataBits = 8;
	MG::thcSerial->StopBits = (StopBits)Enum::Parse(StopBits::typeid, "One");
	MG::thcSerial->Parity = (Parity)Enum::Parse(Parity::typeid, "None");

	// If the port can't be opened, catch the exception, note it and kill the thread.
	try
	{
		// Setting name must be in try in case name isn't defined.
		MG::thcSerial->PortName = MG::thcControl.portName;
		MG::thcSerial->Open();
		MG::thcControl.portOpen = true;

		// Request the current mode.
		//RequestCurrentMode();

		while (!MG::thcControl.hariKari)
		{
			// Initialize the loop variable.
			buffer[0] = 0;

			// Read bytes until we get the header byte or we're killed.
			while (buffer[0] != 0xff)
			{
				// Read the byte that should be a sync byte.
				ReadSomeBytes(buffer, 0, 1);
			}

			// Get the message header byte.		
			ReadSomeBytes(buffer, 0, 1);

			// Read the rest of the message.
			switch ((THC_RESPONSE) buffer[0])
			{
			case THC_RESP_SYNC:
				break;

			case THC_RESP_MODE:
				// Get the rest of the message.
				ReadSomeBytes(buffer, 1, 2);
				// Make sure the checksum is good.
				if (buffer[1] != buffer[2])
				{
					MG::thcData.checksumErrorCnt++;
					break;
				}

				// If we're in Operating mode, the upper nibble has the state.
				if (((THC_MODE)(buffer[1] & 0x0f)) == THC_MODE_OPERATING)
				{
					if ((buffer[1] & 0xf0) != 0)
						MG::thcData.currentState = THC_STATE_CUTTING;
					else
						MG::thcData.currentState = THC_STATE_ENABLED;
				}
				MG::thcData.currentMode = (THC_MODE) (buffer[1]& 0x0f);
				break;

			case THC_RESP_STATUS:
				break;

			case THC_RESP_UNIT:
				// Get the rest of the message.
				ReadSomeBytes(buffer, 1, 2);
				if (buffer[1] == buffer[2])
					MG::thcData.currentUnit = (THC_UNIT) buffer[1];
				else
					MG::thcData.checksumErrorCnt++;
				break;

			case THC_RESP_SETPOINT:
				// Get the rest of the message. 
				ReadSomeBytes(buffer, 1, 3);
				value = ( (((UInt16) (buffer[1] & MSB_COUNTS_MASK)) << 8) | ((UInt16) buffer[2]) );
				checksum = buffer[1] + buffer[2];
				// See if the message was corrupted
				if (checksum == buffer[3])
					;//MG::thcData.currentSetPoint = value;
				else
					MG::thcData.checksumErrorCnt++;
				break;

			case THC_RESP_CURRENT:
				// Get the rest of the message.
				ReadSomeBytes(buffer, 1, 3);
				value = (UInt16)( ( ((UInt16) (buffer[1] & MSB_COUNTS_MASK)) << 8) | ((UInt16) buffer[2]));
				checksum = buffer[1] + buffer[2];
				// Check the checksum.
				if (checksum == buffer[3])
					MG::thcData.currentVoltage = value;
				else
					MG::thcData.checksumErrorCnt++;
				break;

			case THC_RESP_START_CUT:
				// Handle the start of cut.
				break;

			case THC_RESP_CAPTURE_OFF:
				MG::thcData.captureOn = false;
				break;

			case THC_RESP_CAPTURE_ON:
				MG::thcData.captureOn = true;
				break;

	#ifdef NOPE 
			case THC_RESP_FAST_VOLT:
				// Get the rest of the message.
				ReadSomeBytes(buffer, 1, 5);
				decodeFastVoltageOrCounts(buffer);
				break;
	#endif

			case THC_RESP_CUT_PACKET:
				// Get the rest of the message.
				ReadSomeBytes(buffer, 1, 7);

				// We now have all seven bytes to process.
				decodeFullVoltageMessage(buffer);
				break;

			default:
				// just fall through.
				MG::thcData.checksumErrorCnt++;
				break;
			} // Switch

		}  // While !HariKari

		// Close the port and set the flag to show it is closed.
		MG::thcSerial->Close();
		MG::thcControl.portOpen = false;

	} // try

	catch (...)
	{
		MG::thcControl.portOpen = false;
		// Call the exit thread function to clean up the thread resources.
		ExitThread(0);
	}


	// Call the exit thread function to clean up the thread resources.
	ExitThread(0);
}

