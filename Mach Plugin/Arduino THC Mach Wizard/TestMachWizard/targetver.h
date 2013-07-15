#pragma once

// The following macros define the minimum required platform.  The minimum required platform
// is the earliest version of Windows, Internet Explorer etc. that has the necessary features to run 
// your application.  The macros work by enabling all features available on platform versions up to and 
// including the version specified.

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.

#ifndef _DEFAULT_WIN			// Specifies the minimum required platform
//#define	_DEFAULT_WIN 0x0600		// windows vista
#define _DEFAULT_WIN 0x0501		// windows xp
//#define	_DEFAULT_WIN 0x0410		// windows 98 
#endif

#ifndef WINVER
#define WINVER	_DEFAULT_WIN
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT	_DEFAULT_WIN
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS _DEFAULT_WIN
#endif

#ifndef _WIN32_IE				// Specifies that the minimum required platform is Internet Explorer 7.0.
#define _WIN32_IE 0x0700		// Change this to the appropriate value to target other versions of IE.
#endif
