#include "SimulationMap.hpp"
#include "SimulationObject.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <utility>
#include <optional>
#include <cstdlib>
#include <ctime>

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

    objectInCurrentPlace.setGridPosition(row, col);
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
    nextBarrageMissileNumber = 1;
    consoleOut.write("SimulationMap: reset map state.\n");
}

std::optional<std::pair<int, int>> SimulationMap::idToPosition(std::string id) {
    auto itr = positionLookup.find(id);
    if (itr != positionLookup.end()) {
        return itr->second;
    }

    return std::nullopt;
}

std::vector<std::pair<int, int>> SimulationMap::getEmptySpaces() {
    std::vector<std::pair<int, int>> emptySpaces;
    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 16; col++) {
            if (grid[row][col].getType() == SimulationObject::SimulationObjectType::NONE) {
                emptySpaces.push_back({row, col});
            }
        }
    }
    return emptySpaces;
}

std::vector<SimulationMap::SpawnedMissile> SimulationMap::spawnMissileBarrage(int missileNum) {
    std::vector<std::pair<int, int>> emptySpaces = getEmptySpaces();
    std::vector<SpawnedMissile> spawnedMissiles;

    if (missileNum <= 0 || emptySpaces.empty()) {
        return spawnedMissiles;
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for(int i = 0; i < missileNum && !emptySpaces.empty(); i++) {
        const int random = std::rand() % emptySpaces.size();
        std::pair<int, int> selectedSpot = emptySpaces[random];
        std::string id = "missile-barrage-missile-" + std::to_string(nextBarrageMissileNumber++);
        std::string type = "missile-barrage-missile";
        int row = selectedSpot.first;
        int col = selectedSpot.second;

        if (addObject(id, type, row, col)) {
            spawnedMissiles.push_back({id, row, col});
        }

        emptySpaces.erase(emptySpaces.begin() + random);
    }

    return spawnedMissiles;
}

std::vector<SimulationObject> SimulationMap::getProtectedTargets() {
    std::vector<SimulationObject> protectedTargets;
    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 16; col++) {
            if (grid[row][col].getType() == SimulationObject::SimulationObjectType::PROTECTED_TARGET) {
                protectedTargets.push_back(grid[row][col]);
            }
        }
    }
    return protectedTargets;
}
