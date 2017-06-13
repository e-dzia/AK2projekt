#pragma once

#include "Timer.h"
#include <windows.h>

class TimerWindows : public Timer {
    private:
        LARGE_INTEGER time_s;
        LARGE_INTEGER time_e;

        LARGE_INTEGER getTime();
        
    public:
        TimerWindows();
        ~TimerWindows();

        void start();
        void stop();
        double get();
        double getSeconds();
};