/*
 * COPYRIGHT 2013 - Regis M. George
 *
 * License: Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
 *
 * THCDisplay.h
 *
 *  Created on: Sep 3, 2012
 *      Author: Rege George
 */

#ifndef THCDISPLAY_H_
#define THCDISPLAY_H_

#include "LiquidCrystal.h"
#include "Button.hpp"

enum Menu_e
{
	MENU_OPERATING = 0,
	MENU_USE = 1,
	SUBMENU_USE_TIME = 8,
	SUBMENU_USE_ARC_CNT = 9,
	SUBMENU_USE_PLUNGE_CNT = 10,
	SUBMENU_USE_FLASH_CNT = 11,
	SUBMENU_USE_FLASH_BLOCK = 12,
	SUBMENU_USE_RESET = 13,
	SUBMENU_USE_FLASH_RESET = 14,

	MENU_DEFAULT_MODE = 2,
	SUBMENU_DEFAULT_MODE_SET = 15,

	MENU_TEST = 3,
	SUBMENU_TEST_TORCH = 16,
	SUBMENU_TEST_TORCH_TOGGLE = 17,
	SUBMENU_TEST_ARC = 18,
	SUBMENU_TEST_ARC_TOGGLE = 19,
	SUBMENU_TEST_THC_UP = 20,
	SUBMENU_TEST_THC_UP_TOGGLE = 21,
	SUBMENU_TEST_THC_DOWN = 22,
	SUBMENU_TEST_THC_DOWN_TOGGLE = 23,

	MENU_INFO = 4,
	SUBMENU_INFO = 24,

	MENU_DISABLED = 5,

	MENU_BYPASS = 6,

	MENU_CRUISE_MODE = 7,

	MENU_LAST = SUBMENU_INFO
};




enum TorchState_e
{
	TORCH_STATE_UP,
	TORCH_STATE_DOWN,
	TORCH_STATE_GOOD,
	TORCH_STATE_NONE,
	TORCH_STATE_KERF
};




class THCInterface
{
	private:
		// Declare the lcd handle and initialize it with our specific hardware configuration.
		LiquidCrystal *lcd; //(8, 9, 4, 5, 6, 7);

		Menu_e currentMenu;
		Button *selectButton, *cancelButton, *upButton, *downButton,
				*modeButton;
		unsigned int tgtVolt, curVolt;
//		bool tgtChange, curChange;
		bool torchOn, arcGood;
		TorchState_e torchState;
		THCState_e currentTHCState;
		bool flashWritePending;

		void Display_FlashWritePending(void);

	public:
		THCInterface();
		~THCInterface();

		void splash(char *version);
		void initDisplay();
		void clear(void);

		void setMenu(Menu_e newMenu);
		Menu_e getMenu(void);

		Command_e processButtons(void);
		Command_e processUpDownButtons(void);
		Command_e processCruiseButtons(void);
		bool modeChangedPressed(void);

		void TorchOn(bool state);
		void Display_Operating_TorchError();

		void ArcGood(bool state);
		void Display_Operating_ArcError();
		void Display_TargetVoltage(unsigned int voltage);

		void setFlashWritePending(bool pending);

		void setTHCState(THCState_e currentState);

		void Display_THCState(void);

		void SetTorchState(TorchState_e newState);

		void Display_TorchState(void);

		void Display_CurrentVoltage(unsigned int voltage);
		void ForceDisplay_CurrentVoltage(unsigned int voltage);
		void Update_Cutting_Display();
};

#endif /* THCDISPLAY_H_ */
