#pragma once
#include "SimulationOptions.hpp"
#include "WebSocketServer.hpp"
#include "HttpServer.hpp"
#include "ThreadSafeOutput.hpp"
#include "MessageHandler.hpp"
#include "SimulationWorld.hpp"
#include "components/Radar.hpp"
#include "components/Missile.hpp"

#include <nlohmann/json.hpp>
#include <optional>
#include <thread>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <atomic>

using json = nlohmann::json;

class MissileDefenseSimulator {
    public:
        MissileDefenseSimulator();

        void createWebSocketServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port);
        void createHttpServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port);
        void run();
    
    private:
        // Pieces (for now) - later change to own processes
        std::vector<Radar> radars;
        //std::vector<CommandCenter> commandCenters;
        //std::vector<ProtectedTarget> protectedTargets;
        //std::vector<Interceptor> interceptors;
        std::vector<Missile> missiles;
        //std::vector<Tree> trees;
        //std::vector<Lake> lakes;
        // barrages will spawn 100 missiles in random places

        // protected output streams
        ThreadSafeOutput consoleOut = ThreadSafeOutput(std::cout);
        ThreadSafeOutput consoleErr = ThreadSafeOutput(std::cerr);
        
        SimulationOptions options;                      // house all options
        SimulationWorld world;                          // house true world state
        //ProcessManger manager;          // control child-process lifecycle
        //ComponentRegistry registry;     // house endpoints for components
        std::optional<WebSocketServer> webServer;           // front-end back-end communication
        std::optional<HttpServer> httpServer;               // front-end http get requests
        //messageRouter router;           // route messages to components

        // queues for data transmisstion
        ThreadSafeQueue<nlohmann::json> incomingMessages;
        ThreadSafeQueue<nlohmann::json> outGoingMessages;

        void gameLoop();
        
        // CALL BACK FUNCTIONS ------------------
    
        void sendUpdate(const json & message);
    
        // Simulation Control 
        void startSimulation(const json & message);
        void pauseSimulation(const json & message);
        void resetSimulation(const json & message);
        
        // Options Handling
        void updateAutoMode(const json & message);
        void updateRadarVis(const json & message);
        void updateSimSpeed(const json & message);

        // Placement Logic
        void addPiece(const json & message);
        
        // handle incoming/outgoing messages
        // job: match messages to corresponding functions in simulator
        std::optional<MessageHandler> handler;
        void createMessageHandler();

        // Data --------------------------

        std::atomic<bool> running;
        std::atomic<bool> simRunning;
        std::optional<std::thread> simulationThread;

        // Helper Functions --------------
        bool openBrowser(const std::string& url);
        void stopSimulationLoop();

};
