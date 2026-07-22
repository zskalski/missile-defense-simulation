#pragma once

#include "SimulationObject.hpp"
#include "ThreadSafeOutput.hpp"

#include <array>
#include <unordered_map>
#include <string>
#include <utility>
#include <optional>

class SimulationMap {
    public:
        SimulationMap(ThreadSafeOutput & output, ThreadSafeOutput & error);
        
        bool addObject(std::string id, std::string type, int row, int col);
        bool removeObject(std::string id);

    private:
        std::optional<std::pair<int, int>> idToPosition(std::string id);

        ThreadSafeOutput & consoleOut;
        ThreadSafeOutput & consoleErr;

        std::array<std::array<SimulationObject, 16>, 16> grid {};
        std::unordered_map<std::string, std::pair<int, int>> positionLookup;
};