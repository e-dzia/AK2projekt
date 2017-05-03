#pragma once

class Timer {
    protected:
        enum State_t {
            TIMER_NONE,
            TIMER_STARTED,
            TIMER_DONE
        };

    protected:
        State_t time_state;

    public:
        Timer() : time_state(TIMER_NONE) {}
        virtual ~Timer() {}

        virtual void start() = 0;
        virtual void stop() = 0;
        virtual double get() = 0;
        virtual double getSeconds() = 0;
};