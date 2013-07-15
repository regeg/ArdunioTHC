//---------------------------------------------------------------------
//
//	MachDevice.h - header file for the MachDevice dll
//
//		except for the to first two #define statements below
//		this should not need to be changed
//
//		most user code should go in the main app program
//
//---------------------------------------------------------------------

#pragma once

#define	PI_PLUGIN_CPP
#define	PI_PLUGINNAME	"ArduinoTHC"

//---------------------------------------------------------------------
// the includes we need for the mach data structures
//---------------------------------------------------------------------

#include "Engine.h"					// cross platform Engine
#include "rs274ngc.h"				// cross platform RS274NGC
#include "_TrajectoryControl.h"		// cross platform TrajectoryControl
#include "_Mach4View.h"				// cross platform Mach4View

//---------------------------------------------------------------------
// Here are the typedefs of the pointers to functions that can be called
//---------------------------------------------------------------------

typedef void	(_cdecl *VoidShort) (short);
typedef int		(_cdecl *IntShort) (short);
typedef double	(_cdecl *DoubleShort) (short);
typedef void	(_cdecl *VoidShortDouble) (short, double);
typedef bool	(_cdecl *BoolShort) (short);
typedef void	(_cdecl *VoidLPCSTR) (LPCTSTR);
typedef void	(_cdecl *VoidShortBool) (short, bool);

//=====================================================================
//
//	global variables provided by MachDevice.cpp - mainly pointers
//
//	see MachDevice.cpp for documentation
//
//=====================================================================

extern VoidShort			DoButton;		// void		DoButton(short code)
extern IntShort				GetMenuRange;	// int		GetMenuRange(short count)
extern DoubleShort			GetDRO;			// double	GetDRO(short code)
extern VoidShortDouble		SetDRO;			// void		SetDRO(short code, double value);
extern BoolShort			GetLED;			// bool		GetLED(short code);
extern VoidShortBool		SetLED;			// void		SetLED(short code, bool value);
extern VoidLPCSTR			Code;			// void		Code("G0X10Y10");

extern char	*ProfileName;					// file specification for the XML file

extern TrajBuffer			*Engine;		// Ring0 memory for printer port control and other device syncronisation
extern setup				*_setup;		// Trajectory planners setup block. Always in effect
extern _TrajectoryControl	*MainPlanner;	// used for most planner functions and program control 
extern _CMach4View			*MachView;		// used for most framework and configuration calls
											// therefore not very usable by a plugin
