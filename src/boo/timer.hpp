#pragma once

#include <chrono>

class FrameTimer {
public:
    FrameTimer(int fps);

    float delta() const;
    int operator()();
    void relax();

    void reset();

private:
    using Clock = std::chrono::high_resolution_clock;

    Clock::duration _frameDuration;
    float _delta = 0.f;

    Clock::time_point _start = Clock::now();
    size_t _currentFrame = 0;
};