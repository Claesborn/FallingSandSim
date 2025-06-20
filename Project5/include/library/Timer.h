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

#endif
