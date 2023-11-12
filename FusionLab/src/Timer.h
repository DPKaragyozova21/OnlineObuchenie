#pragma once

#include <atomic>
#include <chrono>

struct Timer
{
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;

    float& deltaTime;

    Timer(float& deltaT) :
        deltaTime(deltaT)
    {
        start = std::chrono::high_resolution_clock::now();
    }

    void GetDeltaTime()
    {
        duration = start - std::chrono::high_resolution_clock::now();
        deltaTime = duration.count() * 1000.0f;

        start = std::chrono::high_resolution_clock::now();
    }
};