
#ifndef PLATFORM_H
#define PLATFORM_H

#define MICRO_THC

#ifdef MICRO_THC
#define SERIAL Serial1
#else
#define SERIAL Serial
#endif


#endif
