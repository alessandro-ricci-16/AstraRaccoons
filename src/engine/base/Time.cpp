#include "../../../headers/engine/base/Time.hpp"

std::chrono::steady_clock::time_point Time::lastTime = std::chrono::steady_clock::now();
float Time::delT = 0.0f;

void Time::computeDeltaT() {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> deltaT = currentTime - lastTime;
    lastTime = currentTime;
    delT = deltaT.count();
}

float Time::getDeltaT() {
    return delT;
}

void Time::pRoFiLinG() {
    computeDeltaT();
    std::cout << delT << "\n";
}