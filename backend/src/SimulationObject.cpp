#include "SimulationObject.hpp"

const std::unordered_map<std::string, SimulationObject::SimulationObjectType> SimulationObject::simulationTypes = {
    {"command-center", SimulationObjectType::COMMAND_CENTER},
    {"radar", SimulationObjectType::RADAR},
    {"protected-target", SimulationObjectType::PROTECTED_TARGET},
    {"interceptor", SimulationObjectType::INTERCEPTOR},
    {"enemy-missile", SimulationObjectType::ENEMY_MISSILE},
    {"enemy-missile-barrage", SimulationObjectType::ENEMY_MISSILE_BARRAGE},
    {"tree", SimulationObjectType::TREE},
    {"lake", SimulationObjectType::LAKE}
};

SimulationObject::SimulationObjectType SimulationObject::findSimulationType(const std::string& type) {
    const auto it = simulationTypes.find(type);

    if (it != simulationTypes.end()) {
        return it->second;
    }

    return SimulationObjectType::NONE;
}

bool SimulationObject::setID(const std::string& identification) {
    
    // check to see if id is in format: type-number
    // ex. tree-1 or command-center-1

    auto index = identification.rfind('-');
    
    if (index == std::string::npos) 
        return false;

    std::string type = identification.substr(0, index);

    if(simulationTypes.find(type) == simulationTypes.end())
        return false;

    id = identification;
    return true;
}

bool SimulationObject::setType(const std::string& type) {
    const auto it = simulationTypes.find(type);

    if (it == simulationTypes.end()) {
        return false;
    }

    this->type = it->second;
    return true;
}

SimulationObject::SimulationObject() {
    id = "";
    type = SimulationObjectType::NONE;
}

SimulationObject::SimulationObject(std::string identification, SimulationObjectType t) : id(identification), type(t) {}

SimulationObject::SimulationObjectType SimulationObject::getType() {
    return this->type;
}