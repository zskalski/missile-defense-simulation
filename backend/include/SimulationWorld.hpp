#pragma once
#include "SimulationTimer.hpp"
#include "SimulationMap.hpp"
#include "SimulationOptions.hpp"
#include "ThreadSafeOutput.hpp"
#include "components/Radar.hpp"
#include "components/Missile.hpp"
#include "components/DetectedTarget.hpp"

#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

class SimulationWorld {
    public:
        SimulationWorld(
            ThreadSafeOutput & output,
            ThreadSafeOutput & error,
            SimulationOptions & options,
            std::vector<Radar> & radars,
            std::vector<Missile> & missiles
        );

        // Timer Operations
        void startTimer();
        SimulationTimer::Time getTime();
        void pauseTimer();
        void resetTimer();

        bool addPiece(const json & message);
        bool removePiece(std::string id);
        void reset();
        void update();
        void pause();
        json getPieceTotals() const;
        std::vector<DetectedTarget> getDetectedTargets() const;
        void resetPieceTotals();
        void updateRadarVis();

    private:
        void addPieceToTotals(const std::string & type);
        void addPieceToVector(const json & message);
        void spawnMissileBarrage(int missileNum, const std::string & targetID);

        ThreadSafeOutput & consoleOut;
        ThreadSafeOutput & consoleErr;
        SimulationOptions & options;
        std::vector<Radar> & radars;
        std::vector<Missile> & missiles;
        std::vector<DetectedTarget> detectedTargets;
        SimulationMap map;                  // 2d map of the placed sprites
        SimulationTimer timer;              // stores execution time of simulation
        std::mutex timeMutex;       

        int totalCommandCenters;
        int totalRadars;
        int totalTargets;
        int totalInterceptors;
        int totalEnemyMissiles;
        int totalEnemyMissileBarrages;
        int totalTrees;
        int totalLakes;
        
};
