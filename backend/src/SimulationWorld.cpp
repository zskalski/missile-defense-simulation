#include "SimulationWorld.hpp"
#include <algorithm>

namespace {
    constexpr int mapCellCount = 16;
    constexpr int mapPixelSize = 940;
    constexpr int defaultBarrageMissileSpeed = 10;

    int cellCenterCoordinate(int index) {
        return static_cast<int>(((index + 0.5) * mapPixelSize) / mapCellCount);
    }
}

SimulationWorld::SimulationWorld(
    ThreadSafeOutput & output,
    ThreadSafeOutput & error,
    SimulationOptions & simOptions,
    std::vector<Radar> & radarComponents,
    std::vector<Missile> & missileComponents
)
    : consoleOut(output),
      consoleErr(error),
      options(simOptions),
      radars(radarComponents),
      missiles(missileComponents),
      map(output, error),
      totalCommandCenters(0),
      totalRadars(0),
      totalTargets(0),
      totalInterceptors(0),
      totalEnemyMissiles(0),
      totalEnemyMissileBarrages(0),
      totalTrees(0),
      totalLakes(0) {}

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

bool SimulationWorld::addPiece(const json & message) {
    const json payload = message.at("payload");

    std::string id = payload.at("id").get<std::string>();
    std::string type = payload.at("type").get<std::string>();

    const json position = payload.at("position");
    int row = position.at("row").get<int>();
    int col = position.at("column").get<int>();

    // must ensure missile has valid target before allowing placement
    if (type == "enemy-missile" &&
        (payload.at("target_id") == "0" || payload.at("speed").get<int>() == 0 || payload.at("x_dest").get<int>() == 0 || payload.at("y_dest").get<int>() == 0)) {
        // reject missiles that do not have destination targets
        return false;
    }

    if (type == "enemy-missile-barrage" && payload.at("missile_count") < 2) {
        return false;
    }

    // only one command center is allowed
    if (type == "command-center" && totalCommandCenters > 0) {
        return false;
    }

    const bool placed = map.addObject(id, type, row, col);

    if (placed) {
        addPieceToTotals(type);
        addPieceToVector(message);

        if (type == "enemy-missile-barrage") {
            spawnMissileBarrage(payload.at("missile_count").get<int>(), id);
        }
    }

    return placed;
}

bool SimulationWorld::removePiece(std::string id) {
    return map.removeObject(id);
}

void SimulationWorld::reset() {
    resetTimer();
    resetPieceTotals();
    map.reset();
    radars.clear();
    missiles.clear();
    detectedTargets.clear();
    consoleOut.write("SimulationWorld: reset world state.\n");
}

void SimulationWorld::pause() {
    std::lock_guard<std::mutex> lock(timeMutex);
    timer.pause(); 
}

void SimulationWorld::update() {
    for (auto missile = missiles.begin(); missile != missiles.end();) {
        if (missile->isBlownUp() && missile->wasBlownUpReported()) {
         missile = missiles.erase(missile);
        } else {
            ++missile;
        }
    }

    for (auto & missile : missiles) {
        const bool wasBlownUp = missile.isBlownUp();

        missile.advance();

        if (!wasBlownUp && missile.isBlownUp()) {
            map.removeObject(missile.getID());
        }
    }

    detectedTargets.clear();

    for (auto & missile : missiles) {
        if (missile.isBlownUp()) {
            continue;
        }

        for (auto & radar : radars) {
            if (radar.canDetect(missile)) {
                detectedTargets.emplace_back(missile.getX(), missile.getY(), missile.getSpeed());
                break;
            }
        }
    }

    for (auto & missile : missiles) {
        if (missile.isBlownUp()) {
            missile.markBlownUpReported();
        }
    }
}

json SimulationWorld::getPieceTotals() const {
    return {
        {"commandCenters", totalCommandCenters},
        {"radars", totalRadars},
        {"targets", totalTargets},
        {"interceptors", totalInterceptors},
        {"enemyMissiles", totalEnemyMissiles},
        {"enemyMissileBarrages", totalEnemyMissileBarrages},
        {"trees", totalTrees},
        {"lakes", totalLakes}
    };
}

std::vector<DetectedTarget> SimulationWorld::getDetectedTargets() const {
    return detectedTargets;
}

void SimulationWorld::resetPieceTotals() {
    totalCommandCenters = 0;
    totalRadars = 0;
    totalTargets = 0;
    totalInterceptors = 0;
    totalEnemyMissiles = 0;
    totalEnemyMissileBarrages = 0;
    totalTrees = 0;
    totalLakes = 0;
}

void SimulationWorld::updateRadarVis() {
    for (int i = 0; i < radars.size(); i++) {
        (radars[i]).setRange(options.getRadarVis());
    }
}

void SimulationWorld::addPieceToTotals(const std::string & type) {
    if (type == "command-center") {
        totalCommandCenters += 1;
    } else if (type == "radar") {
        totalRadars += 1;
    } else if (type == "protected-target") {
        totalTargets += 1;
    } else if (type == "interceptor") {
        totalInterceptors += 1;
    } else if (type == "enemy-missile") {
        totalEnemyMissiles += 1;
    } else if (type == "enemy-missile-barrage") {
        totalEnemyMissileBarrages += 1;
    } else if (type == "tree") {
        totalTrees += 1;
    } else if (type == "lake") {
        totalLakes += 1;
    }
}

void SimulationWorld::addPieceToVector(const json & message) {
    const json payload = message.at("payload");
    std::string id = payload.at("id").get<std::string>();
    std::string type = payload.at("type").get<std::string>();

    if (type == "radar") {
        radars.emplace_back(payload.at("x").get<int>(), payload.at("y").get<int>(), options.getRadarVis(), id);
        consoleOut.write("Recieved radar placement with these values:\n\tx: ", payload.at("x").get<int>(), "\n\ty: ", payload.at("y").get<int>(), "\n\tradarVis: ", options.getRadarVis(), "\n\tid: ", id, '\n');
    } else if (type == "enemy-missile") {
        missiles.emplace_back(
            payload.at("x").get<int>(),
            payload.at("y").get<int>(),
            payload.at("speed").get<int>(),
            payload.at("x_dest").get<int>(),
            payload.at("y_dest").get<int>(),
            payload.at("target_id").get<std::string>(),
            id
        );
        consoleOut.write("Recieved enemy-missile placement with these values:\n\tx: ", payload.at("x").get<int>(), "\n\ty: ", payload.at("y").get<int>(), "\n\tspeed: ", payload.at("speed").get<int>(), "\n\tx_dest: ", payload.at("x_dest").get<int>(), "\n\ty_dest: ", payload.at("y_dest").get<int>(), "\n\ttarget_id: ", payload.at("target_id").get<std::string>(), "\n\tid: ", id, '\n');
    }
}

void SimulationWorld::spawnMissileBarrage(int missileNum, const std::string & targetID) {
    const auto spawnedMissiles = map.spawnMissileBarrage(missileNum);
    auto protectedTargets = map.getProtectedTargets();

    for (const auto & spawnedMissile : spawnedMissiles) {

        // get a random protected target
        const int random = std::rand() % protectedTargets.size();
        auto selectedTarget = protectedTargets[random];

        missiles.emplace_back(
            cellCenterCoordinate(spawnedMissile.col),
            cellCenterCoordinate(spawnedMissile.row),
            defaultBarrageMissileSpeed,
            selectedTarget.getX(),
            selectedTarget.getY(),
            selectedTarget.getID(),
            spawnedMissile.id
        );
    }

    totalEnemyMissiles += spawnedMissiles.size();
}
