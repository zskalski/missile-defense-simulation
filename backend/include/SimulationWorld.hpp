#pragma once
#include "SimulationTimer.hpp"
#include "SimulationMap.hpp"
#include "ThreadSafeOutput.hpp"

#include <mutex>

class SimulationWorld {
    public:
        SimulationWorld(ThreadSafeOutput & output, ThreadSafeOutput & error);

        // Timer Operations
        void startTimer();
        SimulationTimer::Time getTime();
        void pauseTimer();
        void resetTimer();

        bool addPiece(std::string id, std::string type, int row, int col);
        bool removePiece(std::string id);

    private:
        ThreadSafeOutput & consoleOut;
        ThreadSafeOutput & consoleErr;
        SimulationMap map;                  // 2d map of the placed sprites
        SimulationTimer timer;              // stores execution time of simulation
        std::mutex timeMutex;       
        
        
};