#pragma once

#include "Timer.h"
#include <chrono>

class TimerLinux : public Timer {
    private:
        std::chrono::nanoseconds time_s;
        std::chrono::nanoseconds time_e;

        std::chrono::nanoseconds getTime();

    public:
        TimerLinux();
        ~TimerLinux();

        void start();
        void stop();
        double get();
        double getSeconds();
};