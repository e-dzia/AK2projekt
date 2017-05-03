#pragma once

#ifdef _WIN32
	#include "TimerWindows.h"
#else
	#include "TimerLinux.h"
#endif

class TimerFactory {
	public:
		static Timer *createTimer() {
		   	#ifdef _WIN32
		    	return new TimerWindows();
		   	#else
		        return new TimerLinux();
		    #endif
		}
};
