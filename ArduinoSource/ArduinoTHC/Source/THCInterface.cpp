/*
* COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
*/

/*
 * THCDisplay.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: Rege
 */

#include "Arduino.h"

#include "THC.h"
#include "THCInterface.hpp"
#include "IOConfig.h"
#include "Button.hpp"

#define OKAY_SYM		0
#define CANCEL_SYM		1
#define UP_BUTTON_SYM	2
#define DOWN_BUTTON_SYM	3
#define UP_TORCH_SYM	4
#define DOWN_TORCH_SYM	5

static const char okaySym[] =
		{
				B11111,
				B11111,
				B11110,
				B11101,
				B01011,
				B10111,
				B11111,
				B11111
		};

static const char cancelSym[] =
		{
				B11111,
				B01110,
				B10101,
				B11011,
				B10101,
				B01110,
				B11111,
				B11111
		};

static const char upButtonSym[] =
		{
				B11111,
				B11011,
				B10001,
				B01010,
				B11011,
				B11011,
				B11011,
				B11011
		};

static const char downButtonSym[] =
		{
				B11011,
				B11011,
				B11011,
				B11011,
				B01010,
				B10001,
				B11011,
				B11111
		};
static const char upArrowSym[] =
		{
				B00100,
				B01110,
				B10101,
				B00100,
				B00100,
				B00100,
				B00100,
				B00000
		};

static const char downArrowSym[] =
		{
				B00100,
				B00100,
				B00100,
				B00100,
				B10101,
				B01110,
				B00100,
				B00000
		};

//
// Define all the text for the menu names so it can just be reference by index.
//
// Note: C99 Style initializers would be nice, but the compiler doesn't do them.
static const char* MenuNames[] =
	{
	"0. Operating", 	// MENU_OPERATING
			"1. Tip Usage...", 	// MENU_USE
			"2. Default Mode.", // MENU_DEFAULT_MODE
			"3. Test...",		// MENU_TEST
			"4. Info...",		// MENU_INFO
			"----DISABLED----", // Mode: MENU_DISABLED
			"BYPASS----------",	// Mode: MENU_BYPASS
			"**Cruise**", 		// Mode: MENU_CRUISE_MODE

			// Sub Menus
			"1.1 Use Time",		// SUBMENU_USE_TIME
			"1.2 Pilot Arc",	// SUBMENU_USE_ARC
			"1.3 Plunge Count", // SUBMENU_USE_PLUNGE
			"1.4 Flash Writes", // SUBMENU_USE_FLASH
			"1.5 Cur Block", 	// SUBMENU_USE_BLOCK
			"1.6 Reset Use", 	// SUBMENU_USE_USE_RESET
			"1.7 Reset Flash", 	// SUBMENU_USE_FLASH_RESET

			"2.1 Set Mode:",	// SUBMENU_DEFAULT_MODE_SET

			"3.1 Torch On",		// SUBMENU_TEST_TORCH
			"Toggle Trch: ", 	// SUBMENU_TEST_TORCH_TOGGLE

			"3.2 Arc Good", 	// SUBMENU_TEST_ARC
			"Toggle Arc:  ", 	// SUBMENU_TEST_ARC_TOGGLE

			"3.3 Torch Up", 	// SUBMENU_TEST_THC_UP
			"Toggle T-UP: ", 	// SUBMENU_TEST_THC_UP_TOGGLE

			"3.4 Torch Down",	// SUBMENU_TEST_THC_DOWN
			"Toggle T-DN: ", 	// SUBMENU_TEST_THC_DOWN_TOGGLE
		};

const char* MenuPrompt[] =
	{
			"", 			// MENU_OPERATING
			" for info", 	// MENU_USE
			" to set mode", // MENU_DEFAULT_MODE
			" to test", 	// MENU_TEST
			" for info", 	// MENU_INFO
			"----------------",	// MENU_DISABLED
			"",				// MENU_BYPASSED
			"",				// MENU_CRUISE_MODE

			// Sub Menus
			"",					// SUBMENU_USE_TIME
			"Total: ",			// SUBMENU_USE_PIERCE
			"Total: ",			// SUBMENU_USE_PLUNGE
			"Total: ",			// SUBMENU_USE_FLASH
			"Block In Use: ",	// SUBMENU_USE_BLOCK
			" to reset",		// SUBMENU_USE_USE_RESET
			" to reset",		// SUBMENU_USE_FLASH_RESET

			"",					// SUBMENU_DEFAULT_MODE_SET

			" to test", 		// SUBMENU_TEST_TORCH
			"",					// SUBMENU_TEST_TORCH_TOGGLE

			" to test",			// SUBMENU_TEST_ARC
			"", 				// SUBMENU_TEST_ARC_TOGGLE

			" to test",			// SUBMENU_TEST_THC_UP
			"",					// SUBMENU_TEST_THC_UP_TOGGLE

			" to test", 		// SUBMENU_TEST_THC_DOWN
			"",					// SUBMENU_TEST_THC_DOWN_TOGGLE

			"",					// SUBMENU_INFO
		};

/*
 This LCD configuration is for the SainSmart 1602 module (16 characters x 2 lines, w/ keypad):
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 */




THCInterface::THCInterface()
	{
	lcd = new LiquidCrystal(8, 9, 4, 5, 6, 7);

	// Define up the LCD's number of columns and rows:
	lcd->begin(16, 2);
	// clear the screen.
	lcd->clear();

	//
	// Create the custom characters for the menu system.
	lcd->createChar(OKAY_SYM, (uint8_t *) okaySym);
	lcd->createChar(CANCEL_SYM, (uint8_t *) cancelSym);
	lcd->createChar(UP_BUTTON_SYM, (uint8_t *) upButtonSym);
	lcd->createChar(DOWN_BUTTON_SYM, (uint8_t *) downButtonSym);
	lcd->createChar(UP_TORCH_SYM, (uint8_t *) upArrowSym);
	lcd->createChar(DOWN_TORCH_SYM, (uint8_t *) downArrowSym);

	//
	// Initialize all the input buttons.
	//

	selectButton = new Button((char *) "Select", IN_D_BUTTON_SELECT,
			IN_D_BUTTON_SELECT_ACTIVE,
			350, // Debounce time Ms.
			500, // Repeat start delay Ms.
			50); // Repeat every Ms.

	cancelButton = new Button((char *) "Cancel", IN_D_BUTTON_CANCEL,
			IN_D_BUTTON_CANCEL_ACTIVE,
			350, // Debounce time Ms.
			500, // Repeat start delay Ms.
			50); // Repeat every Ms.

	downButton = new Button((char *) "Down", IN_D_BUTTON_DOWN,
			IN_D_BUTTON_DOWN_ACTIVE,
			250, // Debounce time Ms.
			500, // Repeat start delay Ms.
			50); // Repeat every Ms.

	upButton = new Button((char *) "Up", IN_D_BUTTON_UP,
			IN_D_BUTTON_UP_ACTIVE,
			250, // Debounce time Ms.
			500, // Repeat start delay Ms.
			50); // Repeat every Ms.

	modeButton = new Button((char *) "Mode", IN_D_BUTTON_MODE,
			IN_D_BUTTON_UP_ACTIVE,
			500,	// Debounce time Ms.
			0, 		// Repeat start delay Ms.
			0); 	// Repeat every Ms.


	//
	// Initialze flags and display state variables.
	torchOn = false;
	arcGood = false;
	torchState = TORCH_STATE_NONE;
	flashWritePending = false;
	// Set the voltage to an invalid value so a valid value will be recognized.
	curVolt = 1023;
	}




THCInterface::~THCInterface()
	{
	}




void THCInterface::splash(char *version)
	{
	lcd->setCursor(0, 0);
	lcd->print("Rege-o-matic");
	lcd->setCursor(0, 1);
	lcd->print("Plasma I/F: V");
	lcd->setCursor(13, 1);
	lcd->print(version);
	}




void THCInterface::initDisplay()
	{
	// Initialize the LCD display with the fixed values
	lcd->clear();
	lcd->setCursor(0, 0);
	lcd->print("T:");
	lcd->setCursor(0, 1);
	lcd->print("A:");
	lcd->setCursor(4, 0);
	lcd->print("TGT:");
	lcd->setCursor(4, 1);
	lcd->print("NOW:");
	//Display_Operating_TargetVoltage(0);
	//Display_Operating_CurrentVoltage(0);

	}




Menu_e THCInterface::getMenu(void)
	{
	return currentMenu;
	}




void THCInterface::TorchOn(bool state)
	{
	torchOn = state;

	if ( (currentMenu == MENU_OPERATING) ||
		 (currentMenu == MENU_BYPASS) ||
		 (currentMenu == MENU_CRUISE_MODE))
		{
		lcd->setCursor(2, 0);
		if (torchOn)
			lcd->print("1");
		else
			lcd->print("0");
		}
	}




void THCInterface::Display_Operating_TorchError()
	{
	lcd->setCursor(2, 0);
	lcd->print("*");
	}




void THCInterface::SetTorchState(TorchState_e newState)
	{

	if (newState == torchState)
		return;

	torchState = newState;
	Display_TorchState();
	}




void THCInterface::Display_TorchState(void)
	{

	switch (torchState)
		{
		case TORCH_STATE_UP:
			// Set the cursor location.
			lcd->setCursor(15, 1);
			lcd->write((uint8_t) UP_TORCH_SYM);
			break;

		case TORCH_STATE_DOWN:
			// Set the cursor location.
			lcd->setCursor(15, 1);
			lcd->write((uint8_t) DOWN_TORCH_SYM);
			break;

		case TORCH_STATE_GOOD:
			// Set the cursor location.
			lcd->setCursor(14, 1);
			lcd->print("OK");
			break;

		case TORCH_STATE_KERF:
			// Set the cursor location.
			lcd->setCursor(14, 1);
			lcd->print("KF");
			break;

		case TORCH_STATE_NONE:
			// Set the cursor location.
			lcd->setCursor(14, 1);
			lcd->print("--");
			break;
		}
	}




void THCInterface::setTHCState(THCState_e currentState)
	{
	if (currentState == currentTHCState)
		return;

	currentTHCState = currentState;

	Display_THCState();
	}




void THCInterface::Display_THCState(void)
	{
	lcd->setCursor(13, 0);

	switch (currentTHCState)
		{
		case THC_STATE_ENABLED:
			lcd->print("ENA");
			break;
		case THC_STATE_CUTTING:
			lcd->print("CUT");
			break;
		}
	}




void THCInterface::ArcGood(bool state)
	{
	arcGood = state;

	if ((currentMenu == MENU_OPERATING) ||
		(currentMenu == MENU_BYPASS) ||
		(currentMenu == MENU_CRUISE_MODE))
		{
		lcd->setCursor(2, 1);
		if (arcGood)
			lcd->print("1");
		else
			lcd->print("0");
		}
	}




void THCInterface::Display_Operating_ArcError()
	{
	lcd->setCursor(2, 1);
	lcd->print("*");
	}




void THCInterface::Display_TargetVoltage(unsigned int voltage)
	{
	lcd->setCursor(8, 0);
	lcd->print("   ");
	lcd->setCursor(8, 0);

	if (voltage > 1023)
		lcd->print("FFF");
	else
		{
		if (voltage < 100)
			lcd->setCursor(9, 0);
		lcd->print(voltage);
		tgtVolt = voltage;
		}
	}




void THCInterface::ForceDisplay_CurrentVoltage(unsigned int voltage)
	{
	curVolt = 0xffff;

	Display_CurrentVoltage(voltage);
	}




void THCInterface::Display_CurrentVoltage(unsigned int voltage)
	{

	if (voltage == curVolt)
		return;

	curVolt = voltage;
	if ((currentMenu != MENU_OPERATING) &&
		(currentMenu != MENU_BYPASS) &&
		(currentMenu != MENU_CRUISE_MODE))
		return;

	lcd->setCursor(8, 1);
	if (curVolt > 999)
		{
		lcd->print("FFF");
		return;
		}

	// Determine how many leading zeros to print.
	if (voltage < 10)
		lcd->print("  ");
	else if (voltage < 100)
		lcd->print(" ");
	lcd->print(voltage);
	}




void THCInterface::clear(void)
	{
	lcd->clear();
	}




void THCInterface::setMenu(Menu_e newMenu)
	{
	DebugPrint("setMenu(<old> ");DebugPrint(currentMenu);DebugPrint(", <new> ");DebugPrint(newMenu);DebugPrintLn(")");

	// If it's already set, don't do anything.
//	if (currentMenu == newMenu)
//		return;
	currentMenu = newMenu;

//	if (currentMenu == MENU_OPERATING)
//		return;
	lcd->clear();
	lcd->setCursor(0, 0);

	switch (currentMenu)
		{
		case MENU_OPERATING:
			lcd->print("T:");
			lcd->setCursor(0, 1);
			lcd->print("A:");
			lcd->setCursor(4, 0);
			lcd->print("TGT:");
			lcd->setCursor(4, 1);
			lcd->print("NOW:");
			Display_TargetVoltage(tgtVolt);
			Display_CurrentVoltage(curVolt);
			TorchOn(currentStateData.torchOnState);
			ArcGood(currentStateData.arcGoodState);
			Display_TorchState();
			Display_THCState();
			Display_FlashWritePending();
			//lcd.print(MenuNames[(int) ])
			break;

		case MENU_USE:
			lcd->print(MenuNames[MENU_USE]);
			lcd->setCursor(0, 1);
			lcd->write((uint8_t) OKAY_SYM);
			lcd->print(MenuPrompt[MENU_USE]);
			break;

		case SUBMENU_USE_TIME:
			lcd->print(MenuNames[SUBMENU_USE_TIME]);
			lcd->setCursor(0,1);
			lcd->print("H:MM:SS ");
			lcd->print(usageData.tipHours);
			lcd->print(":");

			if (usageData.tipMinutes < 10)
				lcd->print("0");
			lcd->print(usageData.tipMinutes);
			lcd->print(":");
			if (usageData.tipSeconds < 10)
				lcd->print("0");
			lcd->print(usageData.tipSeconds);
			break;

		case SUBMENU_USE_ARC_CNT:
			lcd->print(MenuNames[SUBMENU_USE_ARC_CNT]);
			lcd->setCursor(0,1);
			lcd->print(MenuPrompt[SUBMENU_USE_ARC_CNT]);
			lcd->print(usageData.pilotArcCount);
			break;

		case SUBMENU_USE_PLUNGE_CNT:
			lcd->print(MenuNames[SUBMENU_USE_PLUNGE_CNT]);
			lcd->setCursor(0,1);
			lcd->print(MenuPrompt[SUBMENU_USE_PLUNGE_CNT]);
			lcd->print(usageData.pierceCount);
			break;

		case SUBMENU_USE_FLASH_CNT:
			lcd->print(MenuNames[SUBMENU_USE_FLASH_CNT]);
			lcd->setCursor(0,1);
			lcd->print(MenuPrompt[SUBMENU_USE_FLASH_CNT]);
			lcd->print(usageData.initialWriteCount);
			break;

		case SUBMENU_USE_FLASH_BLOCK:
			lcd->print(MenuNames[SUBMENU_USE_FLASH_BLOCK]);
			lcd->setCursor(0,1);
			lcd->print(MenuPrompt[SUBMENU_USE_FLASH_BLOCK]);
			lcd->print(usageData.flashBlock);
			break;

		case SUBMENU_USE_RESET:
			lcd->clear();
			lcd->print(MenuNames[SUBMENU_USE_RESET]);
			lcd->setCursor(0, 1);
			lcd->write((uint8_t) OKAY_SYM);
			lcd->print(MenuPrompt[SUBMENU_USE_RESET]);
			break;

		case SUBMENU_USE_FLASH_RESET:
			lcd->clear();
			lcd->print(MenuNames[SUBMENU_USE_FLASH_RESET]);
			lcd->setCursor(0, 1);
			lcd->write((uint8_t) OKAY_SYM);
			lcd->print(MenuPrompt[SUBMENU_USE_FLASH_RESET]);
			break;

 		case MENU_DEFAULT_MODE:
			DebugPrintLn("setMenu(DEFAULT_MODE)");
			lcd->print(MenuNames[MENU_DEFAULT_MODE]);
			lcd->setCursor(0, 1);
			lcd->write((uint8_t) OKAY_SYM);
			lcd->print(MenuPrompt[MENU_DEFAULT_MODE]);
			break;


		case SUBMENU_DEFAULT_MODE_SET:
			DebugPrintLn("setMenu(SUBMENU_DEFAULT_MODE_SET)");
			lcd->print(MenuNames[SUBMENU_DEFAULT_MODE_SET]);
			lcd->write((uint8_t) UP_BUTTON_SYM);
			lcd->print("/");
			lcd->write((uint8_t) DOWN_BUTTON_SYM);
//			lcd->setCursor(0, 1);
			break;

		case MENU_TEST:
			DebugPrintLn("setMenu(MENU_TEST)");
			lcd->print(MenuNames[MENU_TEST]);
			lcd->setCursor(0, 1);
			lcd->write((uint8_t) OKAY_SYM);
			lcd->print(MenuPrompt[MENU_TEST]);
			break;

		case SUBMENU_TEST_TORCH:
			DebugPrintLn("setMenu(SUBMENU_TEST_TORCH)");
			lcd->print(MenuNames[SUBMENU_TEST_TORCH]);
			lcd->setCursor(0, 1);
			lcd->write((uint8_t) OKAY_SYM);
			lcd->print(MenuPrompt[SUBMENU_TEST_TORCH]);
			break;

		case SUBMENU_TEST_TORCH_TOGGLE:
			DebugPrintLn("setMenu(SUBMENU_TEST_TORCH_TOGGLE)");
			lcd->print(MenuNames[SUBMENU_TEST_TORCH_TOGGLE]);
			lcd->write((uint8_t) UP_BUTTON_SYM);
			lcd->print("/");
			lcd->write((uint8_t) DOWN_BUTTON_SYM);
			break;

		case SUBMENU_TEST_ARC:
			DebugPrintLn("setMenu(SUBMENU_TEST_ARC)");
			lcd->print(MenuNames[SUBMENU_TEST_ARC]);
			lcd->setCursor(0, 1);
			lcd->write((uint8_t) OKAY_SYM);
			lcd->print(MenuPrompt[SUBMENU_TEST_ARC]);
			break;

		case SUBMENU_TEST_ARC_TOGGLE:
			DebugPrintLn("setMenu(SUBMENU_TEST_ARC_TOGGLE)");
			lcd->print(MenuNames[SUBMENU_TEST_ARC_TOGGLE]);
			lcd->write((uint8_t) UP_BUTTON_SYM);
			lcd->print("/");
			lcd->write((uint8_t) DOWN_BUTTON_SYM);

			break;

		case SUBMENU_TEST_THC_UP:
			DebugPrintLn("setMenu(SUBMENU_TEST_THC_UP)");
			DebugPrintLn(MenuNames[SUBMENU_TEST_THC_UP]);
			lcd->print(MenuNames[SUBMENU_TEST_THC_UP]);
			lcd->setCursor(0, 1);
			lcd->write((uint8_t) OKAY_SYM);
			lcd->print(MenuPrompt[SUBMENU_TEST_THC_UP]);
			break;

		case SUBMENU_TEST_THC_UP_TOGGLE:
			DebugPrintLn("setMenu(SUBMENU_TEST_THC_UP_TOGGLE)");
			lcd->print(MenuNames[SUBMENU_TEST_THC_UP_TOGGLE]);
			lcd->write((uint8_t) UP_BUTTON_SYM);
			lcd->print("/");
			lcd->write((uint8_t) DOWN_BUTTON_SYM);
			break;

		case SUBMENU_TEST_THC_DOWN:
			DebugPrintLn("setMenu(SUBMENU_TEST_THC_DOWN)");
			DebugPrintLn(MenuNames[SUBMENU_TEST_THC_DOWN]);
			lcd->print(MenuNames[SUBMENU_TEST_THC_DOWN]);
			lcd->setCursor(0, 1);
			lcd->write((uint8_t) OKAY_SYM);
			lcd->print(MenuPrompt[SUBMENU_TEST_THC_DOWN]);
			break;

		case SUBMENU_TEST_THC_DOWN_TOGGLE:
			DebugPrintLn("setMenu(SUBMENU_TEST_THC_DOWN_TOGGLE)");
			lcd->print(MenuNames[SUBMENU_TEST_THC_DOWN_TOGGLE]);
			lcd->write((uint8_t) UP_BUTTON_SYM);
			lcd->print("/");
			lcd->write((uint8_t) DOWN_BUTTON_SYM);
			break;

		case MENU_INFO:
			DebugPrintLn("setMenu(MENU_INFO)");
			lcd->print(MenuNames[MENU_INFO]);
			lcd->setCursor(0, 1);
			lcd->write((uint8_t) OKAY_SYM);
			lcd->print(MenuPrompt[MENU_INFO]);
			break;

		case SUBMENU_INFO:
			lcd->print("Rege-o-matic");
			lcd->setCursor(0, 1);
			lcd->print("Plasma I/F: V");
			lcd->setCursor(13, 1);
			lcd->print(VERSION);
			break;

		case MENU_DISABLED:
			DebugPrintLn("setMenu(MENU_DISABLED)");
			lcd->setCursor(0, 0);
			lcd->print(MenuNames[MENU_DISABLED]);
			lcd->setCursor(0, 1);
			lcd->print(MenuPrompt[MENU_DISABLED]);
			break;

		case MENU_BYPASS:
			DebugPrintLn("setMenu(MENU_BYPASS)");
			lcd->setCursor(0, 0);
			lcd->print("T:");
			lcd->setCursor(4, 0);
			lcd->print(MenuNames[MENU_BYPASS]);
			lcd->setCursor(0, 1);
			lcd->print("A:");
			lcd->setCursor(4, 1);
			lcd->print("NOW:");
			TorchOn(torchOn);
			ArcGood(arcGood);
			ForceDisplay_CurrentVoltage(curVolt);
			break;

		case MENU_CRUISE_MODE:
			DebugPrintLn("setMenu(MENU_CRUISE_MODE)");
			lcd->setCursor(0, 0);
			lcd->print("T:");
			lcd->setCursor(4, 0);
			lcd->print(MenuNames[MENU_CRUISE_MODE]);
			lcd->setCursor(0, 1);
			lcd->print("A:");
			lcd->setCursor(4, 1);
			lcd->print("NOW:");
			TorchOn(torchOn);
			ArcGood(arcGood);
			ForceDisplay_CurrentVoltage(curVolt);
			break;

		default:
			break;
		}
	}




Command_e THCInterface::processUpDownButtons(void)
	{
	bool upPress, downPress;

	downPress = downButton->WasPressed(BUTTON_REPEAT);

	upPress = upButton->WasPressed(BUTTON_REPEAT);

	if (upPress)
		{
		return COMMAND_VOLT_UP;
		}
	else if (downPress)
		{
		DebugPrint("Got a down press\n");
		return COMMAND_VOLT_DOWN;
		}

	return COMMAND_NONE;
	}




Command_e THCInterface::processCruiseButtons(void)
	{
	bool selectPress;

	selectPress = selectButton->WasPressed(BUTTON_NOREPEAT);

	if (selectPress)
		{
		return COMMAND_CRUISE_SET;
		}

	return COMMAND_NONE;
	}




Command_e THCInterface::processButtons(void)
	{
	bool selectPress, cancelPress, upPress, downPress;
	bool testActive;

	selectPress = selectButton->WasPressed(BUTTON_NOREPEAT);
	cancelPress = cancelButton->WasPressed(BUTTON_NOREPEAT);

	// If multiple buttons active, don't do anything.
	if (selectPress && cancelPress)
		return COMMAND_NONE;

	// Process key strokes based on current interface menu/mode
	switch (currentMenu)
		{
		case MENU_OPERATING:
			if (downButton->WasPressed(BUTTON_REPEAT))
				{
				DebugPrint("COMMAND_VOLT_DOWN\n");
				return COMMAND_VOLT_DOWN;
				}
			else if (upButton->WasPressed(BUTTON_REPEAT))
				{
				DebugPrint("COMMAND_VOLT_UP\n");
				return COMMAND_VOLT_UP;
				}
			else if (selectPress)
				{
				DebugPrintLn("Set Tip Menu");
				setMenu(MENU_USE);
				}
			break;

		case MENU_USE:
			if (cancelPress)
				setMenu(MENU_OPERATING);
			else if (selectPress)
				setMenu(SUBMENU_USE_TIME);
			else if (downButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(MENU_DEFAULT_MODE);
			break;

		case MENU_DEFAULT_MODE:
			if (selectPress)
				setMenu(SUBMENU_DEFAULT_MODE_SET);
			else if (cancelPress)
				setMenu(MENU_OPERATING);
			else if (upButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(MENU_USE);
			else if (downButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(MENU_TEST);
			break;

		case SUBMENU_DEFAULT_MODE_SET:
			// Read each button to clear them.
			selectPress = selectButton->WasPressed(BUTTON_NOREPEAT);
			cancelPress = cancelButton->WasPressed(BUTTON_NOREPEAT);
			upPress = upButton->WasPressed(BUTTON_NOREPEAT);
			downPress = downButton->WasPressed(BUTTON_NOREPEAT);
			lcd->setCursor(3, 1);
			lcd->print("current");

			while (!cancelPress & !selectPress)
				{
				upPress = upButton->WasPressed(BUTTON_NOREPEAT);
				downPress = downButton->WasPressed(BUTTON_NOREPEAT);
				// Are we in the test state with a change request?
				if (upPress)
					{
					lcd->setCursor(3, 1);
					lcd->print("Torch off");
					}
				else if (downPress)
					{
					lcd->setCursor(3, 1);
					lcd->print("Torch on ");
					}
				// Check the select and cancel button to see if pressed.
				selectPress = selectButton->WasPressed(BUTTON_NOREPEAT);
				cancelPress = cancelButton->WasPressed(BUTTON_NOREPEAT);
				}

			if (selectPress)
				{
				// Set the new default mode

				// Return to the main menu.
				setMenu(MENU_OPERATING);
				}
			else
				// Just return to the parent menu.
				setMenu(MENU_DEFAULT_MODE);
			break;

		case MENU_TEST:
			if (selectPress)
				setMenu(SUBMENU_TEST_TORCH);
			else if (cancelPress)
				setMenu(MENU_OPERATING);
			else if (upButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(MENU_DEFAULT_MODE);
			else if (downButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(MENU_INFO);
			break;

		case SUBMENU_TEST_TORCH:
			if (selectPress)
				setMenu(SUBMENU_TEST_TORCH_TOGGLE);
			else if (cancelPress)
				setMenu(MENU_TEST);
			else if (downButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_TEST_ARC);
			break;

		case SUBMENU_TEST_ARC:
			if (selectPress)
				setMenu(SUBMENU_TEST_ARC_TOGGLE);
			else if (cancelPress)
				setMenu(MENU_TEST);
			else if (upButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_TEST_TORCH);
			else if (downButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_TEST_THC_UP);
			break;

		case SUBMENU_TEST_THC_UP:
			if (selectPress)
				setMenu(SUBMENU_TEST_THC_UP_TOGGLE);
			else if (cancelPress)
				setMenu(MENU_TEST);
			else if (upButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_TEST_ARC);
			else if (downButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_TEST_THC_DOWN);
			break;

		case SUBMENU_TEST_THC_DOWN:
			if (selectPress)
				setMenu(SUBMENU_TEST_THC_DOWN_TOGGLE);
			else if (cancelPress)
				setMenu(MENU_TEST);
			else if (upButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_TEST_THC_UP);
			break;

		case SUBMENU_TEST_TORCH_TOGGLE:
			// Read each button to clear them.
			cancelPress = cancelButton->WasPressed(BUTTON_NOREPEAT);
			upPress = upButton->WasPressed(BUTTON_NOREPEAT);
			downPress = downButton->WasPressed(BUTTON_NOREPEAT);
			// Set flag for torch not being active
			testActive = false;
			lcd->setCursor(3, 1);
			lcd->print("Torch off");

			while (!cancelPress)
				{
				upPress = upButton->WasPressed(BUTTON_NOREPEAT);
				downPress = downButton->WasPressed(BUTTON_NOREPEAT);
				// Are we in the test state with a change request?
				if (testActive && upPress)
					{
					// Turn off torch for test
					digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_NOT_ACTIVE);
					lcd->setCursor(3, 1);
					lcd->print("Torch off");
					testActive = false;
					}
				else if (!testActive && downPress)
					{
					// Turn on torch for test
					digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_ACTIVE);
					testActive = true;
					lcd->setCursor(3, 1);
					lcd->print("Torch on ");
					}
				// Check the cancel button to see if pressed.
				cancelPress = cancelButton->WasPressed(BUTTON_NOREPEAT);
				}

			// Make sure the torch is off before leaving.
			// Turn off torch for test
			digitalWrite(OUT_D_TORCH_RELAY, OUT_D_TORCH_RELAY_NOT_ACTIVE);
			setMenu(SUBMENU_TEST_TORCH);
			break;

		case SUBMENU_TEST_ARC_TOGGLE:
			// Read each button to clear them.
			cancelPress = cancelButton->WasPressed(BUTTON_NOREPEAT);
			upPress = upButton->WasPressed(BUTTON_NOREPEAT);
			downPress = downButton->WasPressed(BUTTON_NOREPEAT);
			// Set flag for torch not being active
			testActive = false;
			lcd->setCursor(4, 1);
			lcd->print("Arc off");

			while (!cancelPress)
				{
				upPress = upButton->WasPressed(BUTTON_NOREPEAT);
				downPress = downButton->WasPressed(BUTTON_NOREPEAT);
				// Are we in the test state with a change request?
				if (testActive && upPress)
					{
					// Turn off torch for test
					digitalWrite(OUT_D_ARC_GOOD, OUT_D_ARC_GOOD_NOT_ACTIVE);
					lcd->setCursor(4, 1);
					lcd->print("Arc off");
					testActive = false;
					}
				else if (!testActive && downPress)
					{
					// Turn on torch for test
					digitalWrite(OUT_D_ARC_GOOD, OUT_D_ARC_GOOD_ACTIVE);
					testActive = true;
					lcd->setCursor(4, 1);
					lcd->print("Arc on ");
					}
				// Check the cancel button to see if pressed.
				cancelPress = cancelButton->WasPressed(BUTTON_NOREPEAT);
				}

			// Make sure the torch is off before leaving.
			// Turn off torch for test
			digitalWrite(OUT_D_ARC_GOOD, OUT_D_ARC_GOOD_NOT_ACTIVE);
			setMenu(SUBMENU_TEST_ARC);
			break;

		case SUBMENU_TEST_THC_UP_TOGGLE:
			// Read each button to clear them.
			cancelPress = cancelButton->WasPressed(BUTTON_NOREPEAT);
			upPress = upButton->WasPressed(BUTTON_NOREPEAT);
			downPress = downButton->WasPressed(BUTTON_NOREPEAT);
			// Set flag for torch not being active
			testActive = false;
			lcd->setCursor(3, 1);
			lcd->print("THC UP off");

			while (!cancelPress)
				{
				upPress = upButton->WasPressed(BUTTON_NOREPEAT);
				downPress = downButton->WasPressed(BUTTON_NOREPEAT);
				// Are we in the test state with a change request?
				if (testActive && upPress)
					{
					// Turn off torch for test
					digitalWrite(OUT_D_TORCH_UP, OUT_D_TORCH_UP_NOT_ACTIVE);
					lcd->setCursor(3, 1);
					lcd->print("THC UP off");
					testActive = false;
					}
				else if (!testActive && downPress)
					{
					// Turn on torch for test
					digitalWrite(OUT_D_TORCH_UP, OUT_D_TORCH_UP_ACTIVE);
					testActive = true;
					lcd->setCursor(3, 1);
					lcd->print("THC UP on ");
					}
				// Check the cancel button to see if pressed.
				cancelPress = cancelButton->WasPressed(BUTTON_NOREPEAT);
				}

			// Make sure the torch is off before leaving.
			// Turn off torch for test
			digitalWrite(OUT_D_TORCH_UP, OUT_D_TORCH_UP_NOT_ACTIVE);
			setMenu(SUBMENU_TEST_THC_UP);
			break;

		case SUBMENU_TEST_THC_DOWN_TOGGLE:
			// Read each button to clear them.
			cancelPress = cancelButton->WasPressed(BUTTON_NOREPEAT);
			upPress = upButton->WasPressed(BUTTON_NOREPEAT);
			downPress = downButton->WasPressed(BUTTON_NOREPEAT);
			// Set flag for torch not being active
			testActive = false;
			lcd->setCursor(2, 1);
			lcd->print("THC DOWN off");

			while (!cancelPress)
				{
				upPress = upButton->WasPressed(BUTTON_NOREPEAT);
				downPress = downButton->WasPressed(BUTTON_NOREPEAT);
				// Are we in the test state with a change request?
				if (testActive && upPress)
					{
					// Turn off torch for test
					digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_NOT_ACTIVE);
					lcd->setCursor(2, 1);
					lcd->print("THC DOWN off");
					testActive = false;
					}
				else if (!testActive && downPress)
					{
					// Turn on torch for test
					digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_ACTIVE);
					testActive = true;
					lcd->setCursor(2, 1);
					lcd->print("THC DOWN on ");
					}
				// Check the cancel button to see if pressed.
				cancelPress = cancelButton->WasPressed(BUTTON_NOREPEAT);
				}

			// Make sure the torch is off before leaving.
			// Turn off torch for test
			digitalWrite(OUT_D_TORCH_DOWN, OUT_D_TORCH_DOWN_NOT_ACTIVE);
			setMenu(SUBMENU_TEST_THC_DOWN);
			break;

		case MENU_INFO:
			if (selectPress)
				setMenu(SUBMENU_INFO);
			else if (cancelPress)
				setMenu(MENU_OPERATING);
			else if (upButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(MENU_TEST);
			break;

		case SUBMENU_USE_TIME:
			if (cancelPress)
				setMenu(MENU_USE);
			else if (downButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_USE_ARC_CNT);
			break;

		case SUBMENU_USE_ARC_CNT:
			if (cancelPress)
				setMenu(MENU_USE);
			else if (upButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_USE_TIME);
			else if (downButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_USE_PLUNGE_CNT);
			break;

		case SUBMENU_USE_PLUNGE_CNT:
			if (cancelPress)
				setMenu(MENU_USE);
			else if (upButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_USE_ARC_CNT);
			else if (downButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_USE_FLASH_CNT);
			break;

		case SUBMENU_USE_FLASH_CNT:
			if (cancelPress)
				setMenu(MENU_USE);
			else if (upButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_USE_PLUNGE_CNT);
			else if (downButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_USE_FLASH_BLOCK);
			break;

		case SUBMENU_USE_FLASH_BLOCK:
			if (cancelPress)
				setMenu(MENU_USE);
			else if (upButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_USE_FLASH_CNT);
			else if (downButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_USE_RESET);
			break;

		case SUBMENU_USE_RESET:
			if (selectPress)
				{
				// Return to parent menu so user knows something happened.
				setMenu(MENU_USE);
				// Reset the metrics.
				return COMMAND_RESET_USE_DATA;
				}
			else if (cancelPress)
				setMenu(MENU_USE);
			else if (upButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_USE_FLASH_BLOCK);
			else if (downButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_USE_FLASH_RESET);
			break;

		case SUBMENU_USE_FLASH_RESET:
			if (selectPress)
				{
				// Return to parent menu so user knows something happened.
				setMenu(MENU_USE);
				// Reset the flash block.
				return COMMAND_RESET_FLASH_BLOCK;
				}
			else if (cancelPress)
				setMenu(MENU_USE);
			else if (upButton->WasPressed(BUTTON_NOREPEAT))
				setMenu(SUBMENU_USE_RESET);
			break;

		case SUBMENU_INFO:
			if (selectPress || cancelPress)
				setMenu(MENU_INFO);
			break;

		default:
			DebugPrintLn("invalid menu");
			break;
		}

	// No action command.
	return COMMAND_NONE;
	}



#ifdef NOPE
void THCInterface::Update_Cutting_Display()
	{

	if (tgtChange)
		{
		tgtChange = false;
		Display_TargetVoltage(tgtVolt);
		}

	if (curChange)
		{
		curChange = false;
		Display_CurrentVoltage(curVolt);
		}

	if (arcChange)
		{
		ArcGood(currentStateData.arcGoodState);
		arcChange = false;
		}

	if (torchChange)
		{
		TorchOn(currentStateData.torchOnState);
		torchChange = false;
		}

	}
#endif



void THCInterface::setFlashWritePending(bool pending)
	{

	if (flashWritePending == pending)
		return;

	flashWritePending = pending;
	Display_FlashWritePending();
	}




void THCInterface::Display_FlashWritePending(void)
	{
	if (currentMenu != MENU_OPERATING)
		return;

	lcd->setCursor(12, 0);
	if (flashWritePending)
		lcd->print("*");
	else
		lcd->print(" ");
	}



bool THCInterface::modeChangedPressed(void)
	{
	return modeButton->WasPressed(BUTTON_NOREPEAT);
	}
