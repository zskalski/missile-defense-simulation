#include "SimulationWorld.hpp"

SimulationWorld::SimulationWorld(ThreadSafeOutput & output, ThreadSafeOutput & error)
    : consoleOut(output), consoleErr(error), map(output, error) {}

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

bool SimulationWorld::addPiece(std::string id, std::string type, int row, int col) {
    return map.addObject(id, type, row, col);
}

bool SimulationWorld::removePiece(std::string id) {
    return map.removeObject(id);
}