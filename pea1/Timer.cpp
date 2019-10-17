#include "Timer.h"
#include <sys/time.h>


Timer::Timer()
{

}

void Timer::start() noexcept
{
    gettimeofday(&tv1, NULL);
}

void Timer::stop() noexcept
{
    gettimeofday(&tv2, NULL);
}

double Timer::elapsed() noexcept
{
    return (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec);
}
