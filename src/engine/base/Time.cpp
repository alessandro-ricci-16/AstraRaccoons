#include "../../../headers/engine/base/Time.hpp"

std::chrono::steady_clock::time_point Time::lastTime = std::chrono::steady_clock::now();

float Time::absoluteTime = 0.0f;
float Time::delT = 0.0f;
bool Time::paused = false;
float Time::scale = 1.0f;

void Time::computeDeltaT() {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> deltaT = currentTime - lastTime;
    lastTime = currentTime;
    delT = deltaT.count();
    absoluteTime += delT;
}

float Time::getAbsoluteTime() {
    return absoluteTime;
}

float Time::getDeltaT() {
    return delT * scale;
}

float Time::getScale() {
    return scale;
}

float Time::getFixedDeltaT() {
    return delT;
}

bool Time::isPaused() {
    return paused;
}

void Time::pRoFiLinG() {
    computeDeltaT();
    std::cout << delT << "\n";
}

void Time::setScale(float _scale) {
    scale = _scale;
}

void Time::setPause(bool _paused) {
    if (paused && !_paused)
        lastTime = std::chrono::steady_clock::now();
    paused = _paused;
}