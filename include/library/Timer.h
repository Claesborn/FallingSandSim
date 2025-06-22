#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>
#include <iostream>
#include <string>

struct Timer {
    using Clock = std::chrono::high_resolution_clock;
    std::string       label;
    Clock::time_point start;

    Timer(const std::string& name = "")
        : label(name), start(Clock::now()) {}

    ~Timer() {
        auto end = Clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(
                end - start);
        std::cout << "[TIMER] " << label << " took "
                  << duration.count() / 1000.0 << " ms"
                  << std::endl;
    }
};

struct FPS
{
    using Clock = std::chrono::high_resolution_clock;
    std::string label;
    Clock::time_point start;
    int frameCount = 0;
    double totalTime = 0.0;

    FPS(const std::string& name = "")
        : label(name), start(Clock::now()) {
    }

    void update() {
        frameCount++;
        auto now = Clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start);
        totalTime = duration.count();

        if (totalTime >= 1.0) {
            // Average FPS over 1 sec
            double fps = frameCount / totalTime;
            std::cout << "[FPS] " << label << " avg FPS: " << fps << std::endl;

            // Reset for the next sec
            frameCount = 0;
            start = now;
        }
    }
};

#endif
