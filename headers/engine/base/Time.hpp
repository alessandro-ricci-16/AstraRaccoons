#ifndef __TIME_
#define __TIME_

#include "includes.hpp"

class Time {
private:
    static std::chrono::steady_clock::time_point lastTime;
    static float delT;

public:
    static void computeDeltaT();
    static float getDeltaT();
    static void pRoFiLinG();
};

#endif //__TIME_