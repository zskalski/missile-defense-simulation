#include "SimulationWorld.hpp"

// Timer Operations
void SimulationWorld::startTimer() { 
    std::lock_guard<std::mutex> lock(timeMutex);
    timer.start(); 
}

SimulationTimer::Time SimulationWorld::getTime() { 
    std::lock_guard<std::mutex> lock(timeMutex);
    return timer.getTime(); 
}

void SimulationWorld::pauseTimer() { 
    std::lock_guard<std::mutex> lock(timeMutex);
    timer.pause(); 
}

void SimulationWorld::resetTimer() { 
    std::lock_guard<std::mutex> lock(timeMutex);
    timer.reset(); 
}