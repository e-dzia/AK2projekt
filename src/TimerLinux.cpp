#include "TimerLinux.h"

using namespace std;

TimerLinux::TimerLinux() : Timer() {

}

TimerLinux::~TimerLinux() {

}

std::chrono::nanoseconds TimerLinux::getTime() {
	auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(
		std::chrono::high_resolution_clock::now().time_since_epoch()
	);
    return time;
}

void TimerLinux::start() {
    time_s = getTime();
    time_state = TIMER_STARTED;
}

void TimerLinux::stop() {
    if(time_state != TIMER_STARTED)
        return;

    time_e = getTime();
    time_state = TIMER_DONE;
}

double TimerLinux::get() {
    if(time_state != TIMER_DONE)
        return -1.f;

    return (time_e - time_s) / std::chrono::nanoseconds(1);
}


double TimerLinux::getSeconds() {
    double time = get();
    if(time < 0) return -1.f;
    return time / 1000000000.f;
}
