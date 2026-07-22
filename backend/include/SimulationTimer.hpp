#pragma once
#include <chrono>

class SimulationTimer {
    public:
        SimulationTimer();

        struct Time {
            Time(int h, int m, int s) {
                hours = h;
                minutes = m;
                seconds = s;
            }
            int hours;
            int minutes;
            int seconds;
        };
    
        void start();
        Time getTime();
        void pause();
        void reset();

    private:
        bool isPaused;
        std::chrono::high_resolution_clock::time_point startTime;
        std::chrono::milliseconds pausedTime;
};