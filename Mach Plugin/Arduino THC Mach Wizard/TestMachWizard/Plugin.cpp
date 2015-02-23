//=====================================================================
//
//	Plugin.cpp - the optional custom part of the plugin
//
//	this source file can be filled with the actual custom code that
//	makes the plugin work. it is the choice of the developer to enable
//	which functions will be used from the available MachDevice calls.
//
//	if this is a mixed mode dll each function can be declared as either
//	an unmanaged or managed function.
//
//	please see the documentation in Plugin.h for the #define statements
//	that control each functions compilation.
//
//	if this is a mixed mode dll and you need to keep global managed
//	reference please see the MG class in ManagedGlobal.h
//
//	please read the notes and comments in MachDevice.cpp for general
//	information and disclaimers.
//
//=====================================================================

#include "stdafx.h"
#include "Plugin.h"
#include "MachDevice.h"
#include "ConfigDialog.h"
#include "PlugInControlDialog.h"
#include "XMLNetProfile.h"
#include "THCSerialInterface.h"
#include "ManagedGlobal.h"

#include "ComError.h"

#include <stdlib.h>

//---------------------------------------------------------------------
//	the default namespace for managed functions and classes
//---------------------------------------------------------------------

using namespace ArduinoTHC;

//---------------------------------------------------------------------
//	data area
//---------------------------------------------------------------------

int	menuStart;				// the starting menu id





//---------------------------------------------------------------------
//
//	piInitControl() - Plugin extension of InitControl()
//
//		XML file can NOT be accessed since SetProName hasn't
//		been called yet
//
//		called EVEN if plugin is disabled
//
//---------------------------------------------------------------------
#ifdef PI_INITCONTROL
#ifdef _MANAGED
#pragma PI_MIX_INITCONTROL
#endif
bool piInitControl()
{
	menuStart = GetMenuRange(MENU_COUNT);

	//
	// Initialize all of the THC control variables.
	//
	MG::thcData.currentMode = THC_MODE_BYPASS;
	MG::thcData.currentState = THC_STATE_ENABLED;
	MG::thcData.currentUnit = THC_UNIT_COUNT;
    MG::thcData.currentSetPoint = 0;
	MG::thcData.currentVoltage = 0;
	MG::thcData.torchOn = false;
	MG::thcData.arcGood = false;
	MG::thcData.torchUp = false;
	MG::thcData.torchDown = false;
    MG::thcData.voltControlOn = false;
	MG::thcData.checksumErrorCnt = 0;
	MG::thcData.captureOn = false;
	MG::thcControl.portOpen = false;
	MG::thcControl.portName = nullptr;
	//
	// Initialize the configuration option data.
	MG::THCControlEnabled = false;

	return true;
}
#endif




//---------------------------------------------------------------------
//
//	piSetProName() - Plugin extension of SetProName()
//
//		XML file CAN be accessed
//
//		called EVEN if plugin is disabled
//
//---------------------------------------------------------------------
#ifdef PI_SETPRONAME
#ifdef _MANAGED
#pragma PI_MIX_SETPRONAME
#endif
char* piSetProName(LPCSTR name)
{
	XMLNetProfile^	profile = gcnew XMLNetProfile(gcnew String(ProfileName), "ArduinoTHCMachWizardPlugin", true);

	// Load the saved XML configuration data.
	if (profile->Load())
	{
		// Read the saved config data for enabling the THC.
		MG::THCControlEnabled = profile->ReadBool("EnableTHC");
		// Set the Comm Port based on the saved data.  (XX if none, so port open will fail.
		MG::thcControl.portName = gcnew String(profile->ReadString("ComPort", "COM1"));
	}
	else
		MG::thcControl.portName = gcnew String("COM1");

	// Set thread to non-existent.
	MG::hndlSerialThread = 0;
	// Set flag to allow serial thread to run.
	MG::thcControl.hariKari = false;
	MG::thcControl.portOpen = false;

	// Return the name of the plug-in
	return "ArduinoTHC-RegeG-v0.0.0.0";
}
#endif




//---------------------------------------------------------------------
//
//	piPostInitControl() - Plugin extension of PostInitControl()
//
//		XML file can NOT be accessed
//
//		called ONLY if plugin is enabled
//
//---------------------------------------------------------------------
#ifdef PI_POSTINITCONTROL
#ifdef _MANAGED
#pragma PI_MIX_POSTINITCONTROL
#endif
void piPostInitControl()
{
	HMENU			hMachMenu = GetMenu(MachView->MachFrame->m_hWnd);
	HMENU			hPluginMenu = 0;
	int				machMenuCnt = GetMenuItemCount(hMachMenu);
	MENUITEMINFO	mii;
	LPTSTR			txt;


	for (int i = 0; i < machMenuCnt; i++)
	{
		mii.cbSize     = sizeof(MENUITEMINFO);
		mii.fMask      = MIIM_FTYPE | MIIM_ID | MIIM_SUBMENU | MIIM_STRING;
		mii.dwTypeData = NULL;

		if (GetMenuItemInfo(hMachMenu, i, true, &mii))
		{
			txt = (LPTSTR) malloc(++mii.cch);
			mii.dwTypeData = txt;

			if (GetMenuItemInfo(hMachMenu, i, true, &mii))
			{
				if (strcmp(txt, "PlugIn Control") == 0)
				{
					hPluginMenu = mii.hSubMenu;
					i = machMenuCnt;
				}
			}
			free(txt);
		}

		if (hPluginMenu)
		{
			InsertMenu(hPluginMenu, -1, MF_BYPOSITION, menuStart  , "Arduino THC by RegeG");
		}
	}

	// Set the LED to indicate that the THC isn't talking.
	SetLED(LED_THC_STATUS, false);
	// Try to launch the serial thread.
	MG::hndlSerialThread = CreateThread(NULL, 0, SerialProcessing, NULL, 0, 0); 
	// See if the config is set to turn on the THC control on boot
	if (MG::THCControlEnabled)
	{
		Engine->THCOn = true;
	}

}
#endif




//---------------------------------------------------------------------
//
//	piConfig() - Plugin extension of Config()
//
//		called if user presses CONFIG in Config|Config Plugins
//		even if plugin is disabled
//
//		XML file CAN be accessed
//
//---------------------------------------------------------------------
#ifdef PI_CONFIG
#ifdef _MANAGED
#pragma PI_MIX_CONFIG
#endif
void piConfig()
{
	ConfigDialog^	configDialog = gcnew ConfigDialog();

	// This dialog just says to use the plugin config menu.
	configDialog->ShowDialog();
}
#endif




//---------------------------------------------------------------------
//
//	piStopPlug() - Plugin extension of StopPlug()
//
//---------------------------------------------------------------------
#ifdef PI_STOPPLUG
#ifdef _MANAGED
#pragma PI_MIX_STOPPLUG
#endif
void piStopPlug()
{
	XMLNetProfile^	profile = gcnew XMLNetProfile(gcnew String(ProfileName), "ArduinoTHCMachWizardPlugin", true);

	//
	// We're stopping so save any options to the XML file.
	//

// <XML_WEIRDNESS>
//			If you save values to the XML file when changed, but not here, they won't "stick".
// </XML_WEIRDNESS>

	// See if we can read the existing XML file.
	if (profile->Load())
	{
		// Save the updated value to the XML file.
		profile->WriteString("ComPort", MG::thcControl.portName);
		// Save the default THC state.
		profile->WriteBool("EnableTHC", MG::THCControlEnabled);
		// Save the XML file to disk.
		profile->Save();
	}

	// Set the flag to kill the serial processing thread.
	MG::thcControl.hariKari = true;
	// If the thread is running, wait until it dies.
	if (MG::hndlSerialThread != 0)
	{
		WaitForSingleObject(MG::hndlSerialThread, INFINITE); 
	}
}
#endif




//---------------------------------------------------------------------
//
//	piUpdate() - Plugin extension of Update()
//
//		XML file can NOT be accessed
//
//		called ONLY if plugin is enabled
//
//		WARNING - when you enable a plugin it immediately is added
//		to the update loop. if you haven't initialized some items
//		because PostInitControl() hasn't been called you can get
//		some problems!!!
//
//---------------------------------------------------------------------

#ifdef PI_UPDATE
#ifdef _MANAGED
#pragma PI_MIX_UPDATE
#endif
void piUpdate()
{

	if (MG::thcData.torchOn != MG::thcDataOld.torchOn)
	{
		SetLED(LED_TORCH_ON, MG::thcData.torchOn);
		MG::thcDataOld.torchOn = MG::thcData.torchOn;
	}
	if (MG::thcDataOld.arcGood != MG::thcData.arcGood)
	{
		SetLED(LED_ARC_GOOD, MG::thcData.arcGood);
		MG::thcDataOld.arcGood = MG::thcData.arcGood;
	}
	if (MG::thcDataOld.torchUp != MG::thcData.torchUp)
	{
		SetLED(LED_TORCH_UP, MG::thcData.torchUp);
		MG::thcDataOld.torchUp = MG::thcData.torchUp;
	}
	if (MG::thcDataOld.torchDown != MG::thcData.torchDown)
	{
		SetLED(LED_TORCH_DOWN, MG::thcData.torchDown);
		MG::thcDataOld.torchDown = MG::thcData.torchDown;
	}
	if (MG::thcDataOld.voltControlOn != MG::thcData.voltControlOn)
	{
		SetLED(LED_CONTROL_ACTIVE, MG::thcData.voltControlOn);
		MG::thcDataOld.voltControlOn = MG::thcData.voltControlOn;
	}

	if (!MG::thcControl.portOpen)
	{
		SetLED(LED_THC_STATUS, 1);
		SetDRO((short) DRO_CURRENT, (double) 0);
		SetDRO((short) DRO_SET_POINT, (double) 0);
		SetDRO((short) DRO_CHECKSUM, (double) 0);
		SetLED(LED_UNITS_COUNTS, 0);
		SetLED(LED_UNITS_VOLTS, 0);
		return;
	}

	SetLED(LED_THC_STATUS, 0);

	//if (MG::thcDataOld.currentMode !=  MG::thcData.currentMode)
	{
		// Make sure all LED's are off.
		SetLED(LED_MODE_DISABLED, 0);
		SetLED(LED_MODE_BYPASS, 0);
		SetLED(LED_MODE_CRUISE, 0);
		SetLED(LED_MODE_OPERATING, 0);

		switch (MG::thcData.currentMode)
		{
			case THC_MODE_DISABLED:
				SetLED(LED_MODE_DISABLED, 1);
				break;

			case THC_MODE_BYPASS:
				SetLED(LED_MODE_BYPASS, 1);
				break;

			case THC_MODE_CRUISE:
				SetLED(LED_MODE_CRUISE, 1);
				break;

			case THC_MODE_OPERATING:
				SetLED(LED_MODE_OPERATING, 1);
				break;

		}
		MG::thcDataOld.currentMode =  MG::thcData.currentMode;
	}

	if (MG::thcDataOld.currentUnit != MG::thcData.currentUnit)
	{
		if (MG::thcData.currentUnit == THC_UNIT_VOLT)
		{
			SetLED(LED_UNITS_COUNTS, 0);
			SetLED(LED_UNITS_VOLTS, 1);
		}
		else
		{
			SetLED(LED_UNITS_COUNTS, 1);
			SetLED(LED_UNITS_VOLTS, 0);
		}
		MG::thcDataOld.currentUnit = MG::thcData.currentUnit;
	}

	if (MG::thcDataOld.currentVoltage != MG::thcData.currentVoltage)
	{
		SetDRO((short) DRO_CURRENT, (double) MG::thcData.currentVoltage);
		MG::thcDataOld.currentVoltage = MG::thcData.currentVoltage;
	}
	if (MG::thcDataOld.currentSetPoint != MG::thcData.currentSetPoint)
	{
		SetDRO((short) DRO_SET_POINT, (double) MG::thcData.currentSetPoint);
		MG::thcDataOld.currentSetPoint = MG::thcData.currentSetPoint;
	}
	if ( MG::thcDataOld.checksumErrorCnt != MG::thcData.checksumErrorCnt)
	{
		SetDRO((short) DRO_CHECKSUM, (double) MG::thcData.checksumErrorCnt);
		MG::thcDataOld.checksumErrorCnt =  MG::thcData.checksumErrorCnt;
	}
}
#endif




//---------------------------------------------------------------------
//
//	piNotify() - Plugin extension of Notify()
//
//		among other notices this is where we are notified when the
//		user clicks on our 'PlugIn Control' menu item.
//
//		XML file CAN be accessed on a menu item notify
//
//---------------------------------------------------------------------
#ifdef PI_NOTIFY
#ifdef _MANAGED
#pragma PI_MIX_NOTIFY
#endif
void piNotify(int id)
{
	array<unsigned char>^ buffer = gcnew array<unsigned char>(8);
	unsigned int originalIndex;
	bool portChanged = false;

	//
	// Did the user select the Plug In configuration option?
	if (id == menuStart)
	{
		PlugInControlDialog^	pluginControlDialog = gcnew PlugInControlDialog();

		// Put the current port name in the label for display.
		pluginControlDialog->lblCurrentPort->Text = MG::thcSerial->PortName;
		// Initialize the combo box with the current port selected, if it is in the list of ports present.
		pluginControlDialog->cbComPort->SelectedIndex = pluginControlDialog->cbComPort->Items->IndexOf(MG::thcSerial->PortName);
		// Initialize the configuration for enabling THC control on boot.
		pluginControlDialog->chkTHCEnabled->Checked = MG::THCControlEnabled;
		// Keep track of the selected com port so we can tell if it changed.
		originalIndex = pluginControlDialog->cbComPort->SelectedIndex;

		// Put the dialog box up.  If the user hit "okay", save the changes.
		if (pluginControlDialog->ShowDialog() == DialogResult::OK)
		{
			// See if the port changed.
			if (originalIndex != pluginControlDialog->cbComPort->SelectedIndex)
			{
				// Set the flag so we know it was updated.			
				portChanged = true;

				// Get the selected Com Port name.
				MG::thcControl.portName = pluginControlDialog->cbComPort->SelectedItem->ToString();
			}

			// Get the default state for THC enabled.
			MG::THCControlEnabled = pluginControlDialog->chkTHCEnabled->Checked;
			// See if a Serial Restart was requested.
			if (portChanged)
			{
				// Stop the serial thread.
				MG::thcControl.hariKari = true;
			}
		}
	}

	//
	// We could have also received a button command, but we will ignore them if the
	// serial port isnt' open and we aren't talking to the THC.
	//
	else if (MG::thcControl.portOpen)
	{
		try
		{
			if (id == BUTTON_SETPOINT_UP)
			{
				buffer[0] = (unsigned char) THC_CMD_SETPOINT_UP;
				MG::thcSerial->Write(buffer, 0, 1); 
			}
			else if (id == BUTTON_SETPOINT_DOWN)
			{
				buffer[0] = (unsigned char) THC_CMD_SETPOINT_DOWN;
				MG::thcSerial->Write(buffer, 0, 1);
			}
			else if (id == BUTTON_TOGGLE_UNITS)
			{
				buffer[0] = (unsigned char) THC_CMD_STEP_UNIT;
				MG::thcSerial->Write(buffer, 0, 1);
			}
			else if (id == BUTTON_TOGGLE_MODE)
			{
				buffer[0] = (unsigned char) THC_CMD_STEP_MODE;
				MG::thcSerial->Write(buffer, 0, 1);
			}
		}
		catch (...)
		{
			MG::thcControl.portOpen = false;
		}
	}

}
#endif

//---------------------------------------------------------------------
//
//	piDoDwell() - Plugin extension of DoDwell()
//
//---------------------------------------------------------------------

#ifdef PI_DODWELL
#ifdef _MANAGED
#pragma PI_MIX_DODWELL
#endif
void piDoDwell(double time)
{
}
#endif

//---------------------------------------------------------------------
//
//	piReset() - Plugin extension of Reset()
//
//---------------------------------------------------------------------

#ifdef PI_RESET
#ifdef _MANAGED
#pragma PI_MIX_RESET
#endif
void piReset()
{
}
#endif

//---------------------------------------------------------------------
//
//	piJogOn() - Plugin extension of JogOn()
//
//---------------------------------------------------------------------

#ifdef PI_JOGON
#ifdef _MANAGED
#pragma PI_MIX_JOGON
#endif
void piJogOn(short axis, short dir, double speed)
{
}
#endif

//---------------------------------------------------------------------
//
//	piJogOff() - Plugin extension of JogOff()
//
//---------------------------------------------------------------------

#ifdef PI_JOGOFF
#ifdef _MANAGED
#pragma PI_MIX_JOGOFF
#endif
void piJogOff(short axis)
{
}
#endif

//---------------------------------------------------------------------
//
//	piPurge() - Plugin extension of Purge()
//
//---------------------------------------------------------------------

#ifdef PI_PURGE
#ifdef _MANAGED
#pragma PI_MIX_PURGE
#endif
void piPurge(short flags)
{
}
#endif

//---------------------------------------------------------------------
//
//	piProbe() - Plugin extension of Probe()
//
//---------------------------------------------------------------------

#ifdef PI_PROBE
#ifdef _MANAGED
#pragma PI_MIX_PROBE
#endif
void piProbe()
{
}
#endif

//---------------------------------------------------------------------
//
//	piHome() - Plugin extension of Home()
//
//---------------------------------------------------------------------

#ifdef PI_HOME
#ifdef _MANAGED
#pragma PI_MIX_HOME
#endif
void piHome(short axis)
{
}
#endif
