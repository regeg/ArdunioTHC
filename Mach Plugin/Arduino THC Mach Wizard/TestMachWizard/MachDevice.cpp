//=====================================================================
//
//	MachDevice.cpp - the interface and entry points for the plugin
//
//		version 1.1 - 1 mar 2009
//
//		This file contains the required routines for a Mach Plugin
//		compiled and built in Visual Studio 2008 C++.
//
//		Most of these routines are exposed as entry points into
//		the plugin's dll so Mach can communicate with the plugin.
//
//		It, optionally, calls device dependent code in Plugin.cpp.
//
//		Please read ALL notes in the section and function headers.
//		Also please read ALL notes in the matching MachDevice.h file.
//
//		By configuring the various #define statements in Plugin.h
//		the user can tailor this module to their needs without editing
//		the source code. This will allow for future revisions
//		to more easily drop into place as needed.
//
//		It is designed to work with either: mixed mode .NET,
//		MFC, CLR, or simple stand alone C++. All environments
//		should work in Visual Studio 2008 C++ Standard or above.
//		All environments EXCEPT MFC should work with Visual Studio
//		2008 C++ Express versions.
//
//		This is a TOTALLY reworked version of the MachDevice.cpp
//		that has been created by Art Fenerty, Brian Barker, and John
//		Prentice. This work is a derivative of their hard work and
//		any credit should include them. But any problems are probably
//		mine.
//
//		Many of the techniques used in this code and the examples
//		are based on the hundreds, if not thousands, of hours of time
//		spent by Scott Shafer of S S Systems LLC and I over the last year
//		as we delved very deeply into the plugin mystique.
//
//		SHORT VERSION LEGALIZE: Use at your own risk. The author has
//		released this for use by other Plugin Developers at their
//		discretion and makes no warranties of usability or reliability.
//		It is up to the user to determine the practical uses of this
//		software and its safety and dependability in real world
//		situations.
//
//		I have never liked code with a lot of #define and #ifdef
//		statements. I have tried to keep it to a minimum. Most of
//		the ones in this module are for Managed/Unmanaged and the
//		optional calling of an isolated routine in Plugin.cpp
//
//		All of the defines used are prefaced with 'PI_' and all of the
//		functions calls into external files (Plugin.cpp) are prefaced
//		with 'pi'. This is a departure from the use of 'my' as a prefix
//		in other plugin development tools.
//
//		Author: Edward D. Bryson, Joshua 1 Systems Inc.
//
//=====================================================================

//=====================================================================
//
//	the includes we need to make things work
//
//=====================================================================

#include "stdafx.h"					// VS precompiled header for Windows

#include "MachDevice.h"				// our configuration and globals

#ifdef	PI_PLUGIN_CPP
#include "Plugin.h"					// the optional isolated Plugin Code
#endif

//---------------------------------------------------------------------
//
//	extra includes area (not recommended)
//
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//
//	these routines need to be unmanaged if you are developing in mixed
//	mode. so if /clr is selected then _MANAGED will be defined and we
//	need to force the rest of the code to #pragma unmanaged
//
//---------------------------------------------------------------------

#ifdef	_MANAGED
#pragma unmanaged
#endif

//=====================================================================
//
//	global variables
//
//	mostly pointers to functions and structures withing mach
//
//=====================================================================

//---------------------------------------------------------------------
//
//	function pointers
//
//---------------------------------------------------------------------

VoidShort			DoButton;		// void		DoButton(short code)
IntShort			GetMenuRange;	// int		GetMenuRange(short count)
DoubleShort			GetDRO;			// Double	GetDRO(short code)
VoidShortDouble		SetDRO;			// void		SetDRO(short code, double value);
BoolShort			GetLED;			// bool		GetLED(short code);
VoidShortBool		SetLED;			// void		SetLED(short code, bool value);
VoidLPCSTR			Code;			// void		Code("G0X10Y10");

//---------------------------------------------------------------------
//
//	simple values provided by mach
//
//---------------------------------------------------------------------

char	*ProfileName;		// file specification for the XML file

//---------------------------------------------------------------------
//	
//	structure pointers
//
//	Mach provides the plugin with pointers to several VERY LARGE objects
//	within mach's dataspace. Inside of Mach these are usually large
//	classes which depend on MFC and would not be portable across compiler
//	and MFC library versions. Even if they could be used as classes it
//	would require the use of VS 2008 Standard or above to allow using
//	MFC. To remove these restrictions the pointers are interpreted as
//	structure pointers matching a structure using only intrinsic data
//	types that maps the available data elements.
//
//	Since these are now structures representing ONLY the available data
//	members you cannot call functions based on these structures or map
//	them to MFC classes within the plugin.
//
//	Please see the sample plugins, read the .h files that define these
//	structures and read all available online documentation and forums
//	for more information about the contents and uses of these structures.
//
//---------------------------------------------------------------------

TrajBuffer			*Engine;		// Ring0 memory for printer port control and other device syncronisation
setup				*_setup;		// Trajectory planners setup block. Always in effect
_TrajectoryControl	*MainPlanner;	// used for most planner functions and program control 
_CMach4View			*MachView;		// used for most framework and configuration calls
									// therefore not very usable by a plugin

//=====================================================================
//
//	during mach startup and the loading of all plugins mach will first
//	call the two following functions.
//
//=====================================================================

//---------------------------------------------------------------------
//
//	SetGetMenuRange() - set the 'int GetMenuRange(short count)' pointer
//
//		XML file can NOT be accessed since SetProName hasn't
//		been called yet
//
//		called EVEN if plugin is disabled
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void SetGetMenuRange(IntShort pFunc)
{
   GetMenuRange = pFunc; 
}

//---------------------------------------------------------------------
//
//	InitControl() - called in startup to provide data object pointers
//
//		XML file can NOT be accessed since SetProName hasn't
//		been called yet
//
//		called EVEN if plugin is disabled
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) bool InitControl( void *oEngine , void *oSetup , void *oMainPlanner, void *oView)
{
	bool	startPrinter;		// function return - if true, start the printer port

	Engine		= (TrajBuffer *) oEngine;
	_setup		= (setup *) oSetup;
	MainPlanner = (_TrajectoryControl *) oMainPlanner;
	MachView	= (_CMach4View *) oView;

#ifdef	PI_INITCONTROL
	startPrinter = piInitControl();
#else
	startPrinter = true;
#endif

	return startPrinter;
}

//=====================================================================
//
//	then mach will call each of the following functions with the
//	pointer for the functions which we can directly call inside of
//	mach. each function will simply store the pointer in our global
//	function pointers so they can be called from anywhere inside
//	the plugin. they are currently called in the order in which they
//	appear.
//
//	these routines will probably only need modifying if mach changes
//	its plugin loading procedures
//
//=====================================================================

//---------------------------------------------------------------------
//
//	SetDoButton() - set the 'void DoButton(short code)' pointer
//
//		XML file can NOT be accessed since SetProName hasn't
//		been called yet
//
//		called EVEN if plugin is disabled
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void SetDoButton(VoidShort pFunc)
{
   DoButton = pFunc; 
}

//---------------------------------------------------------------------
//
//	SetSetDRO() - set the 'void SetDRO(short code, double value)' pointer
//
//		XML file can NOT be accessed since SetProName hasn't
//		been called yet
//
//		called EVEN if plugin is disabled
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void SetSetDRO(VoidShortDouble pFunc)
{
   SetDRO = pFunc; 
}

//---------------------------------------------------------------------
//
//	SetGetDRO() - set the 'double GetDRO(short code)' pointer
//
//		XML file can NOT be accessed since SetProName hasn't
//		been called yet
//
//		called EVEN if plugin is disabled
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void SetGetDRO(DoubleShort pFunc)
{
   GetDRO = pFunc; 
}

//---------------------------------------------------------------------
//
//	SetGetLED() - set the 'bool GetLED(short code)' pointer  
//
//		XML file can NOT be accessed since SetProName hasn't
//		been called yet
//
//		called EVEN if plugin is disabled
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void SetGetLED(BoolShort pFunc)
{
   GetLED = pFunc; 
}

//---------------------------------------------------------------------
//
//	SetSetLED() - set the 'void SetLED(short code, bool value)' pointer
//
//		XML file can NOT be accessed since SetProName hasn't
//		been called yet
//
//		called EVEN if plugin is disabled
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void SetSetLED(VoidShortBool pFunc)
{
   SetLED = pFunc; 
}

//---------------------------------------------------------------------
//
//	SetCode() - set the 'void Code(LPCSTR code)' pointer
//
//		XML file can NOT be accessed since SetProName hasn't
//		been called yet
//
//		called EVEN if plugin is disabled
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void SetCode(VoidLPCSTR pFunc)
{
   Code = pFunc; 
}

//---------------------------------------------------------------------
//
//	SetProName() - set xml profile name and get plugin name
//
//		this function performs two purposes. first it supplies the plugin
//		with the file specifier for the xml file. so we first make
//		a local copy of that for use when we want to load/save
//		configuration data in the xml file. the second major purpose
//		is that it returns the plugins proper name to mach for its
//		use.
//
//		XML file CAN be accessed
//
//		called EVEN if plugin is disabled
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) char* SetProName(LPCSTR name)  
{
	char*	pluginName;

	ProfileName = _strdup(name);

#ifdef PI_SETPRONAME
	pluginName = piSetProName(name);
#else
	pluginName = PI_PLUGINNAME;
#endif

	return pluginName;
}

//=====================================================================
//
//	initialization, setup, configuration, and shutdown functions
//
//	mach will call the following functions in its startup and shutdown
//	sequences. these routines are useful for configuring the plugin,
//	creating data objects, initializing and establishing communication
//	with hardware, and then reversing all actions as mach shuts down.
//
//	we have tried to list them in the observed order that mach calls
//	them but this could change. we think of these routines as modal
//	and not time sensitive and it is therefore believed to be safe to
//	perform modal or blocking i/o if needed.
//
//=====================================================================

//---------------------------------------------------------------------
//
//	PostInitControl() - called when startup is pretty well done
//
//		This routine is called after Mach has initialized.
//
//		This routine is handy for changing variables that Mach has
//		loaded at startup. Usually, Mach will permanently save any
//		var's you change here..
//
//		XML file can NOT be accessed!!
//
//		called ONLY if plugin is enabled
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void PostInitControl()
{
#ifdef	PI_POSTINITCONTROL
	piPostInitControl();
#endif
}

//---------------------------------------------------------------------
//
//	Config() - old style config
//
//		This routine is call when the CONFIG button is pressed on
//		the plugin control screen within mach. It can be used for
//		configuring the plugin or the plugin can add menu items
//		to mach's menu and call its own configuration functions
//		from Notify().
//
//		XML file CAN be accessed
//
//		called if user presses CONFIG in Config|Config Plugins
//		even if plugin is disabled
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void Config()
{
#ifdef	PI_CONFIG
	piConfig();
#endif
}

//---------------------------------------------------------------------
//
//	StopPlug() - called in shutdown for plugin to cleanup its act
//
//		This routine should be used to save any information to the
//		XNL file that the plugin wants retained, destroy any objects
//		or resources, and shutdown any communication with hardware.
//
//		XML file CAN be accessed
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void StopPlug(void)
{
#ifdef	PI_STOPPLUG
	piStopPlug();
#endif
}

//=====================================================================
//
//	Update() - 10hz synchonous update loop
//
//		This routine is the meat of a plugin's interaction with
//		mach and/or the outside world. Approximately 10 times per
//		second mach will call this routine and allow it to perform
//		its needed updates. Since this is a continuous load on the
//		system care should be taken to minimize processing requirements
//		and no blocking or modal i/o should be performed.
//
//		XML file can NOT be accessed
//
//		called ONLY if plugin is enabled
//
//=====================================================================

extern "C" __declspec(dllexport) void Update()
{
#ifdef	PI_UPDATE
	piUpdate();
#endif
}

//=====================================================================
//
//	Notify() - message notify
//
//		XML file CAN be accessed on menu id notifies
//
//=====================================================================

extern "C" __declspec(dllexport) void Notify(int id)
{
#ifdef	PI_NOTIFY
	piNotify(id);
#endif
}

//=====================================================================
//
//	the rest of the functions are asynchonously called by mach as needed.
//
//	each function should still be considered time sensitive and care
//	should be taken to minimize processing requiremtns and no blocking
//	or modal i/o should be performed.
//
//=====================================================================

//---------------------------------------------------------------------
//
//	DoDwell() - ???
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void DoDwell(double time)
{
#ifdef	PI_DODWELL
	piDoDwell(time);
#endif
}

//---------------------------------------------------------------------
//
//	Reset() - called when reset is pressed
//
//		Called when reset is pressed, at end of actual reset commend in Mach3. 
//		Check the Engine.Estop variable to see if we have reset or not..
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void Reset()
{
#ifdef	PI_RESET
	piReset();
#endif
}

//---------------------------------------------------------------------
//
//	JogOn() -
//
//		called when Jog is commanded. 0 for speed is jog % jog,
//		otherwise it is a new jog %
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void JogOn(short axis, short dir, double speed)
{
#ifdef	PI_JOGON
	piJogOn(axis, dir, speed);
#endif
}

//---------------------------------------------------------------------
//
//	JogOff() -
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void JogOff(short axis)
{
#ifdef	PI_JOGOFF
	piJogOff(axis);
#endif
}

//---------------------------------------------------------------------
//
//	Purge() - 
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void Purge(short flags)
{
#ifdef	PI_PURGE
	piPurge(flags);
#endif
}

//---------------------------------------------------------------------
//
//	Probe() - 
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void Probe()
{
#ifdef	PI_PROBE
	piProbe();
#endif
}

//---------------------------------------------------------------------
//
//	Home() -
//
//---------------------------------------------------------------------

extern "C" __declspec(dllexport) void Home(short axis)
{
#ifdef	PI_HOME
	piHome(axis);
#endif
}
