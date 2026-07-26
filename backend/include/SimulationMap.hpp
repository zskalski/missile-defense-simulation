#pragma once

#include "SimulationObject.hpp"
#include "ThreadSafeOutput.hpp"

#include <array>
#include <unordered_map>
#include <string>
#include <utility>
#include <optional>
#include <vector>

class SimulationMap {
    public:
        struct SpawnedMissile {
            std::string id;
            int row;
            int col;
        };

        SimulationMap(ThreadSafeOutput & output, ThreadSafeOutput & error);
        
        bool addObject(std::string id, std::string type, int row, int col);
        bool removeObject(std::string id);
        std::vector<SpawnedMissile> spawnMissileBarrage(int missileNum);
        std::vector<SimulationObject> getProtectedTargets();
        void reset();

    private:
        std::optional<std::pair<int, int>> idToPosition(std::string id);
        std::vector<std::pair<int, int>> getEmptySpaces();
        ThreadSafeOutput & consoleOut;
        ThreadSafeOutput & consoleErr;

        std::array<std::array<SimulationObject, 16>, 16> grid {};
        std::unordered_map<std::string, std::pair<int, int>> positionLookup;
        int nextBarrageMissileNumber = 1;
};
