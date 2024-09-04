#include "timer.hpp"

#include <thread>

FrameTimer::FrameTimer(int fps)
    : _frameDuration(
        std::chrono::duration_cast<Clock::duration>(
            std::chrono::duration<double>(1.0 / fps)))
    , _delta(1.f / fps)
{ }

float FrameTimer::delta() const
{
    return _delta;
}

int FrameTimer::operator()()
{
    auto frameIndex = (Clock::now() - _start) / _frameDuration;
    auto framesPassed = static_cast<int>(frameIndex - _currentFrame);
    _currentFrame = frameIndex;
    return framesPassed;
}

void FrameTimer::relax()
{
    std::this_thread::sleep_until(_start + _frameDuration * (_currentFrame + 1));
}

void FrameTimer::reset()
{
    _start = Clock::now();
    _currentFrame = 0;
}