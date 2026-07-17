#include "SimulationOptions.hpp"
#include <iostream>

SimulationOptions::SimulationOptions(bool autoM, int radar, int sim) : 
    autoMode(autoM), radarVis(radar), simSpeed(sim) {}

bool SimulationOptions::getAuto() { return autoMode; }
int SimulationOptions::getRadarVis() { return radarVis; }
int SimulationOptions::getSimSpeed() { return simSpeed; }

bool SimulationOptions::setAuto(bool mode) {
    autoMode = mode;
    return true;
}
    
bool SimulationOptions::setRadarVis(unsigned short vis) {
    if (vis < 1 || vis > 100)
        return false;
    radarVis = vis;
    return true;
}
    
bool SimulationOptions::setSimSpeed(unsigned short speed) {
    if (speed < 1 || speed > 100)
        return false;
    simSpeed = speed;
    return true;
}

void SimulationOptions::printOptions() {
    std::cout 
        << "Auto mode: " << autoMode << '\n'
        << "Radar Visibility: " << radarVis << '\n'
        << "Simulation Speed: " << simSpeed << '\n';
}