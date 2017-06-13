#include "TimerWindows.h"

TimerWindows::TimerWindows() : Timer() {

}

TimerWindows::~TimerWindows() {

}

LARGE_INTEGER TimerWindows::getTime() {
    LARGE_INTEGER timer;
    DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
    QueryPerformanceCounter(&timer);
    SetThreadAffinityMask(GetCurrentThread(), oldmask);
    return timer;
}

void TimerWindows::start() {
    time_s = getTime();
    time_state = TIMER_STARTED;
}

void TimerWindows::stop() {
    if(time_state != TIMER_STARTED)
        return;

    time_e = getTime();
    time_state = TIMER_DONE;
}

double TimerWindows::get() {
    if(time_state != TIMER_DONE)
        return -1.f;

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (double)(time_e.QuadPart - time_s.QuadPart)/frequency.QuadPart;
}

double TimerWindows::getSeconds() {
    return get();
}
