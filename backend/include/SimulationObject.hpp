#pragma once
#include <string>
#include <unordered_map>


class SimulationObject {
    public:
        
    enum SimulationObjectType {
            RADAR,
            PROTECTED_TARGET,
            COMMAND_CENTER,
            INTERCEPTOR,
            TREE,
            LAKE,
            ENEMY_MISSILE,
            ENEMY_MISSILE_BARRAGE,
            NONE
        };

        static SimulationObjectType findSimulationType(const std::string& type);

        SimulationObject(std::string identification, SimulationObjectType t);
        SimulationObject();

        SimulationObjectType getType();
        bool setID(const std::string& identification);
        bool setType(const std::string& type);

    private:
        static const std::unordered_map<std::string, SimulationObjectType> simulationTypes;

        std::string id;
        SimulationObjectType type;
};
