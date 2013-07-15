//=====================================================================
//
//	PI_function definitions
//
//	these #define statement enable the function in Plugin.cpp if defined.
//	MachDevice.cpp also uses these to enable calls to the each function
//	it is defined.
//
//	use these to enable those functions which you wish to implement.
//
//=====================================================================

namespace ArduinoTHC {}				// setup default namespace for .NET

#define PI_INITCONTROL			// piInitControl()

#define PI_SETPRONAME			// piSetProName()

#define PI_POSTINITCONTROL		// piPostInitControl()
#define PI_CONFIG				// piConfig()
#define PI_STOPPLUG				// piStopPlug()
#define PI_UPDATE				// piUpdate()
#define PI_NOTIFY				// piNotify()
//#define PI_DODWELL				// piDoDwell()
//#define PI_RESET				// piReset()
//#define PI_JOGON				// piJogOn()
//#define PI_JOGOFF				// piJogOff()
//#define PI_PURGE				// piPurge()
//#define PI_PROBE				// piProbe()
//#define PI_HOME					// piHome()

//=====================================================================
//
//	PI_MIX_function definition
//
//	these #define statement will define the 'managed' or 'unmanaged'
//	#pragma for each function. they only come into play if the plugin
//	is being built in mixed mode.
//
//	by default we run ALL in managed, of course only if in mixed mode
//
//=====================================================================

#define PI_MIX_INITCONTROL		managed		// piInitControl()

#define PI_MIX_SETPRONAME		managed		// piSetProName()

#define PI_MIX_POSTINITCONTROL	managed		// piPostInitControl()
#define PI_MIX_CONFIG			managed		// piConfig()
#define PI_MIX_STOPPLUG			managed		// piStopPlug

#define PI_MIX_UPDATE			managed		// piUpdate()
#define PI_MIX_NOTIFY			managed		// piNotify()

#define PI_MIX_DODWELL			managed		// piDoDwell()
#define PI_MIX_RESET			managed		// piReset()
#define PI_MIX_JOGON			managed		// piJogOn()
#define PI_MIX_JOGOFF			managed		// piJogOff()
#define PI_MIX_PURGE			managed		// piPurge()
#define PI_MIX_PROBE			managed		// piProbe()
#define PI_MIX_HOME				managed		// piHome()

//=====================================================================
//
//	each function prototype if enable/disable by PI_function.
//
//	if mixed mode each function prototype is #pragma'd as managed
//	or unmanaged by PI_MIX_function
//
//=====================================================================

#ifdef PI_INITCONTROL				// piInitControl()
#ifdef _MANAGED
#pragma PI_MIX_INITCONTROL
#endif
bool piInitControl();
#endif

#ifdef PI_SETPRONAME				// piSetProName()
#ifdef _MANAGED
#pragma PI_MIX_SETPRONAME
#endif
char* piSetProName(LPCSTR name);
#endif

#ifdef PI_POSTINITCONTROL			// piPostInitControl()
#ifdef _MANAGED
#pragma PI_MIX_POSTINITCONTROL
#endif
void piPostInitControl();
#endif

#ifdef PI_CONFIG					// piConfig()
#ifdef _MANAGED
#pragma PI_MIX_CONFIG
#endif
void piConfig();
#endif

#ifdef PI_STOPPLUG					// piStopPlug()
#ifdef _MANAGED
#pragma PI_MIX_STOPPLUG
#endif
void piStopPlug();
#endif

#ifdef PI_UPDATE					// piUpdate()
#ifdef _MANAGED
#pragma PI_MIX_UPDATE
#endif
void piUpdate();
#endif

#ifdef PI_NOTIFY					// piNotify()
#ifdef _MANAGED
#pragma PI_MIX_NOTIFY
#endif
void piNotify(int id);
#endif

#ifdef PI_DODWELL					// piDoDwell()
#ifdef _MANAGED
#pragma PI_MIX_DODWELL
#endif
void piDoDwell(double time);
#endif

#ifdef PI_RESET						// piReset()
#ifdef _MANAGED
#pragma PI_MIX_RESET
#endif
void piReset();
#endif

#ifdef PI_JOGON						// piJogOn()
#ifdef _MANAGED
#pragma PI_MIX_JOGON
#endif
void piJogOn(short axis, short dir, double speed);
#endif

#ifdef PI_JOGOFF					// piJogOff()
#ifdef _MANAGED
#pragma PI_MIX_JOGOFF
#endif
void piJogOff(short axis);
#endif

#ifdef PI_PURGE						// piPurge()
#ifdef _MANAGED
#pragma PI_MIX_PURGE
#endif
void piPurge(short flags);
#endif

#ifdef PI_PROBE						// piProbe()
#ifdef _MANAGED
#pragma PI_MIX_PROBE
#endif
void piProbe();
#endif

#ifdef PI_HOME						// piHome()
#ifdef _MANAGED
#pragma PI_MIX_HOME
#endif
void piHome(short axis);
#endif

//---------------------------------------------------------------------
//
//	plugin's data areas
//
//---------------------------------------------------------------------

#define	MENU_COUNT	1				// # of menu id's to request
extern	int		menuStart;			// base menu id returned
