#pragma once
#include "SimulationTimer.hpp"

#include <mutex>

class SimulationWorld {
    public:

        // Timer Operations
        void startTimer();
        SimulationTimer::Time getTime();
        void pauseTimer();
        void resetTimer();

    private:
        //SimulationMap map;                // 2d map of the placed sprites
        
        SimulationTimer timer;              // stores execution time of simulation
        std::mutex timeMutex;        
};