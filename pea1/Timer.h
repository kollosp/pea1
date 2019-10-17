#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
    struct timeval  tv1, tv2;

public:
    Timer();
    void start() noexcept;
    void stop() noexcept;
    double elapsed() noexcept;
};

#endif // TIMER_H
