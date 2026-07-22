#include "SimulationTimer.hpp"
#include <chrono>

using Clock = std::chrono::steady_clock;

SimulationTimer::SimulationTimer() {
    isPaused = false;
    startTime = Clock::time_point{};
    pausedTime = std::chrono::milliseconds::zero();
}

void SimulationTimer::start() {
    // Timer has never started
    if (startTime == Clock::time_point{}) {
        startTime = Clock::now();
        isPaused = false;
        return;
    }

    // Resume a paused timer
    if (isPaused) {
        startTime = Clock::now();
        isPaused = false;
    }
}

SimulationTimer::Time SimulationTimer::getTime() {
    // Timer has never started
    if (startTime == Clock::time_point{}) {
        return Time(0, 0, 0);
    }

    std::chrono::milliseconds totalMs;

    if (isPaused) {
        totalMs = pausedTime;
    } else {
        auto currentTime = Clock::now();

        totalMs =
            pausedTime +
            std::chrono::duration_cast<std::chrono::milliseconds>(
                currentTime - startTime
            );
    }

    std::chrono::hh_mm_ss hhMmSs(totalMs);

    int hours = static_cast<int>(hhMmSs.hours().count());
    int minutes = static_cast<int>(hhMmSs.minutes().count());
    int seconds = static_cast<int>(hhMmSs.seconds().count());

    return Time(hours, minutes, seconds);
}

void SimulationTimer::pause() {
    if (startTime == Clock::time_point{} || isPaused) {
        return;
    }

    auto currentTime = Clock::now();

    pausedTime +=
        std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - startTime
        );

    isPaused = true;
}

void SimulationTimer::reset() {
    startTime = Clock::time_point{};
    pausedTime = std::chrono::milliseconds::zero();
    isPaused = false;
}