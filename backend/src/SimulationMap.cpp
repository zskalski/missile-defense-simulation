#include "SimulationMap.hpp"
#include "SimulationObject.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <utility>
#include <optional>

    //std::vector<SimulationObject> map;
    //std::unordered_map<std::string, std::pair<int, int>> positionLookup;

SimulationMap::SimulationMap(ThreadSafeOutput & output, ThreadSafeOutput & error)
    : consoleOut(output), consoleErr(error) {}
        
bool SimulationMap::addObject(std::string id, std::string type, int row, int col) {
    
    if(idToPosition(id) != std::nullopt) {
        consoleErr.write("SimulationMap Error: piece already exists: ", id, '\n');
        return false;   // Error: exact piece already in map
    }

    if(row > 15 || row < 0 || col > 15 || col < 0) {
        consoleErr.write("SimulationMap Error: invalid placement coordinates for ", id, " at (", row, ", ", col, ")\n");
        return false;   // Error: invalid row or col
    }

    auto& objectInCurrentPlace = grid[row][col];
    if (objectInCurrentPlace.getType() != SimulationObject::SimulationObjectType::NONE) {
        consoleErr.write("SimulationMap Error: occupied cell at (", row, ", ", col, ")\n");
        return false;   // Error: another piece already exists in that spot
    }

    if (!objectInCurrentPlace.setID(id)) {
        consoleErr.write("SimulationMap Error: invalid id for ", id, '\n');
        return false;
    }
    if (!objectInCurrentPlace.setType(type)) {
        consoleErr.write("SimulationMap Error: invalid type for ", id, " (", type, ")\n");
        return false;
    }

    positionLookup[id] = {row, col};
    consoleOut.write("SimulationMap: placed ", id, " at (", row, ", ", col, ")\n");

    return true;
}

bool SimulationMap::removeObject(std::string id) {
    auto position = idToPosition(id);
    if (!position) {
        consoleErr.write("SimulationMap Error: no piece found for ", id, '\n');
        return false;
    }

    int row = position->first;
    int col = position->second;
    auto& objectInCurrentPlace = grid[row][col];
    
    if (objectInCurrentPlace.getType() == SimulationObject::SimulationObjectType::NONE) {
        consoleErr.write("SimulationMap Error: empty cell for ", id, '\n');
        return false;
    }

    objectInCurrentPlace = SimulationObject{};
    positionLookup.erase(id);
    consoleOut.write("SimulationMap: removed ", id, " from (", row, ", ", col, ")\n");

    return true;
}

void SimulationMap::reset() {
    grid = {};
    positionLookup.clear();
    consoleOut.write("SimulationMap: reset map state.\n");
}

std::optional<std::pair<int, int>> SimulationMap::idToPosition(std::string id) {
    auto itr = positionLookup.find(id);
    if (itr != positionLookup.end()) {
        return itr->second;
    }

    return std::nullopt;
}

        
