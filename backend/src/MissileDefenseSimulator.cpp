#include "MissileDefenseSimulator.hpp"
#include "ThreadSafeQueue.hpp"
#include "WebSocketServer.hpp"

#include <nlohmann/json.hpp>
#include <thread>
#include <iostream>
#include <filesystem>
#include <cstdint>
#include <windows.h>
#include <shellapi.h>
#include <string>

MissileDefenseSimulator::MissileDefenseSimulator()
    : incomingMessages(), outGoingMessages(), running(false), world(consoleOut, consoleErr) {
        totalCommandCenters = 0;
        totalRadars = 0;
        totalTargets = 0;
        totalInterceptors = 0;
        totalEnemyMissiles = 0;
        totalEnemyMissileBarrages = 0;
        totalTrees = 0;
        totalLakes = 0;
}

void MissileDefenseSimulator::createWebSocketServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port) {
    // construct WebSocketServer in-place to avoid copying/moving
    webServer.emplace(ctx, address, port, incomingMessages, outGoingMessages, consoleOut, consoleErr);
}

void MissileDefenseSimulator::createHttpServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port) {
    httpServer.emplace(ctx, address, port, std::filesystem::path{"frontend"}, consoleOut, consoleErr);
}

bool MissileDefenseSimulator::openBrowser(const std::string& url) {

        const HINSTANCE result = ShellExecuteA(
        nullptr,
        "open",
        url.c_str(),
        nullptr,
        nullptr,
        SW_SHOWNORMAL
    );

    return reinterpret_cast<std::intptr_t>(result) > 32;
}

void MissileDefenseSimulator::run() {

    if (!webServer) {
        std::cerr << "Simulator error: web server has not been created. Server start aborted.\n"; 
        return;
    }

    if (!httpServer) {
        std::cerr << "Simulator error: http server has not been created. Server start aborted.\n"; 
        return;
    }

    createMessageHandler();

    if (!handler) {
        std::cerr << "Simulator error: message handler not been created. Server start aborted.\n"; 
        return;
    }

    running = true;

    std::thread httpServerThread([this]() { httpServer->run(); });
    std::thread webServerThread([this]() { webServer->run(); });
    std::thread messageProcessingThread([this]() { handler->run(); });

    std::string url = "http://" + httpServer->getEndpoint().address().to_string() + ':' + std::to_string(httpServer->getEndpoint().port());
    openBrowser(url);

    // populate options with default values
    options = SimulationOptions(false, 50, 1);

    messageProcessingThread.join();
    webServerThread.join();
    httpServerThread.join();
}



// State Updates ---------------------

void MissileDefenseSimulator::sendUpdate(const json & message) {
    auto t = world.getTime();
    
    json updateJson = {
        {"type", "update.response"},
        {"payload", {
            {"timer", {
                {"hours", t.hours},
                {"minutes", t.minutes},
                {"seconds", t.seconds}
            }},
            {"totalPieces", {
                {"commandCenters", totalCommandCenters},
                {"radars", totalRadars},
                {"targets", totalTargets},
                {"interceptors", totalInterceptors},
                {"enemyMissiles", totalEnemyMissiles},
                {"enemyMissileBarrages", totalEnemyMissileBarrages},
                {"trees", totalTrees},
                {"lakes", totalLakes}
            }},
            {"tracks", {
                {"total", 0}
            }}
        }}
    };

    //consoleOut.write("Sending updateJson: ", updateJson.dump(), '\n');
    outGoingMessages.push(updateJson);
}



// Simulation Control ------------------

void MissileDefenseSimulator::startSimulation(const json & message) {
    world.startTimer();

    json startJson = {
        {"type", "start.response"},
        {"payload", {
            // empty for now
        }}
    };
    consoleOut.write("Sending startJson: ", startJson.dump(), '\n');
    outGoingMessages.push(startJson);
}

void MissileDefenseSimulator::pauseSimulation(const json & message) {
    world.pauseTimer();

    json pauseJson = {
        {"type", "pause.response"},
        {"payload", {
            // empty for now
        }}
    };
    consoleOut.write("Sending pauseJson: ", pauseJson.dump(), '\n');
    outGoingMessages.push(pauseJson);
}

void MissileDefenseSimulator::resetSimulation(const json & message) {
    world.resetTimer();

    totalCommandCenters = 0;
    totalRadars = 0;
    totalTargets = 0;
    totalInterceptors = 0;
    totalEnemyMissiles = 0;
    totalEnemyMissileBarrages = 0;
    totalTrees = 0;
    totalLakes = 0;

    json resetJson = {
        {"type", "reset.response"},
        {"payload", {
            // empty for now
        }}
    };
    consoleOut.write("Sending resetJson: ", resetJson.dump(), '\n');
    outGoingMessages.push(resetJson);
}



// User Options -------------

void MissileDefenseSimulator::updateAutoMode(const json & message) {
    options->setAuto((message["payload"]["doAuto"]));
    json reply = {
        {"type", "doAuto.response"},
        {"payload", {
            {"doAuto", message["payload"]["doAuto"]}
        }}
    };
    consoleOut.write("Sending doAuto reply: ", reply.dump(), '\n');
    outGoingMessages.push(reply);
}

void MissileDefenseSimulator::updateRadarVis(const json & message) {
    options->setRadarVis(message["payload"]["radarVis"]);
    json reply = {
        {"type", "radarVis.response"},
        {"payload", {
            {"radarVis", message["payload"]["radarVis"]}
        }}
    };
    consoleOut.write("Sending radarVis reply: ", reply.dump(), '\n');
    outGoingMessages.push(reply);
}

void MissileDefenseSimulator::updateSimSpeed(const json & message) {
    options->setSimSpeed(message["payload"]["simSpeed"]);
    json reply = {
        {"type", "simSpeed.response"},
        {"payload", {
            {"simSpeed", message["payload"]["simSpeed"]}
        }}
    };
    consoleOut.write("Sending simSpeed reply: ", reply.dump(), '\n');
    outGoingMessages.push(reply);
}



// Placement Validation ---------------------

void MissileDefenseSimulator::addPiece(const json & message) {
    const json payload = message.at("payload");

    std::string id = payload.at("id").get<std::string>();
    std::string type = payload.at("type").get<std::string>();

    const json position = payload.at("position");
    int row = position.at("row").get<int>();
    int col = position.at("column").get<int>();

    const bool placed = world.addPiece(id, type, row, col);
    
    if (placed) {
        addPieceToTotals(type);
        consoleOut.write(id, " placed at row: ", row, ", col: ", col, '\n');
    } else {
        consoleOut.write("Placement error: ", id, " could not be placed at row: ", row, ", col: ", col, '\n');
    }

    json reply = {
        {"type", "placement.response"},
        {"payload", {
            {"id", id},
            {"status", placed}
        }}
    };

    outGoingMessages.push(reply);

    // Expected JSON structure:
    // {
    //   "type": "placement.request",
    //   "payload": {
    //     "id": "radar-1",
    //     "type": "radar",
    //     "position": {
    //       "row": 3,
    //       "col": 4
    //     }
    //   }
    // }
}

void MissileDefenseSimulator::addPieceToTotals(const std::string type) {
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


void MissileDefenseSimulator::createMessageHandler() {
    handler.emplace(

        consoleOut,
        consoleErr, 
        incomingMessages,

        [this](const json& message) {
            this->sendUpdate(message);
        },

        [this](const json& message) {
            this->startSimulation(message);
        },

        [this](const json& message) {
            this->pauseSimulation(message);
        },

        [this](const json& message) {
            this->resetSimulation(message);
        },

        [this](const json& message) {
            this->updateAutoMode(message);
        },

        [this](const json& message) {
            this->updateRadarVis(message);
        },

        [this](const json& message) {
            this->updateSimSpeed(message);
        },

        [this](const json& message) {
            this->addPiece(message);
        }
    );
    consoleOut.write("Message handler created.\n");
}