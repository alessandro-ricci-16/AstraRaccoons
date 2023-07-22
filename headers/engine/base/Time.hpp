#ifndef __TIME_
#define __TIME_

#include "includes.hpp"

class Time {
private:
    static std::chrono::steady_clock::time_point lastTime;
    static float absoluteTime;
    static float delT;
    static float scale;

public:
    static void computeDeltaT();
    static float getAbsoluteTime();
    static float getDeltaT();
    static float getFixedDeltaT();
    static float getScale();
    static void pRoFiLinG();
    static void setScale(float scale);
};

#endif //__TIME_