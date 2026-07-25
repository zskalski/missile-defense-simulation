#include "SimulationOptions.hpp"

#include <iostream>
#include <mutex>

SimulationOptions::SimulationOptions(bool autoM, int radar, int sim) : 
    autoMode(autoM), radarVis(radar), simSpeed(sim) {}

bool SimulationOptions::getAuto() const {
    std::lock_guard<std::mutex> lock(optionsMutex);
    return autoMode;
}

int SimulationOptions::getRadarVis() const {
    std::lock_guard<std::mutex> lock(optionsMutex);
    return radarVis;
}

int SimulationOptions::getSimSpeed() const {
    std::lock_guard<std::mutex> lock(optionsMutex);
    return simSpeed;
}

bool SimulationOptions::setAuto(bool mode) {
    std::lock_guard<std::mutex> lock(optionsMutex);
    autoMode = mode;
    return true;
}
    
bool SimulationOptions::setRadarVis(unsigned short vis) {
    std::lock_guard<std::mutex> lock(optionsMutex);
    
    if (vis < 1 || vis > 100)
        return false;
    
    radarVis = vis;
    return true;
}
    
bool SimulationOptions::setSimSpeed(unsigned short speed) {
    std::lock_guard<std::mutex> lock(optionsMutex);
    
    if (speed < 1 || speed > 100)
        return false;

    simSpeed = speed;
    return true;
}

void SimulationOptions::printOptions() const {
    std::lock_guard<std::mutex> lock(optionsMutex);
    std::cout 
        << "Auto mode: " << autoMode << '\n'
        << "Radar Visibility: " << radarVis << '\n'
        << "Simulation Speed: " << simSpeed << '\n';
}
