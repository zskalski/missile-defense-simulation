#pragma once
#include "SimulationOptions.hpp"
#include "WebSocketServer.hpp"
#include "HttpServer.hpp"
#include "ThreadSafeOutput.hpp"
#include "MessageHandler.hpp"
#include "SimulationWorld.hpp"

#include <nlohmann/json.hpp>
#include <optional>
#include <thread>
#include <iostream>
#include <unordered_map>

using json = nlohmann::json;

class MissileDefenseSimulator {
    public:
        MissileDefenseSimulator();

        void createWebSocketServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port);
        void createHttpServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port);
        void run();

    private:
        
        std::optional<SimulationOptions> options;       // house all options
        SimulationWorld world;                          // house true world state
        //ProcessManger manager;          // control child-process lifecycle
        //ComponentRegistry registry;     // house endpoints for components
        std::optional<WebSocketServer> webServer;           // front-end back-end communication
        std::optional<HttpServer> httpServer;               // front-end http get requests
        //messageRouter router;           // route messages to components
        

        // queues for data transmisstion
        ThreadSafeQueue<nlohmann::json> incomingMessages;
        ThreadSafeQueue<nlohmann::json> outGoingMessages;

        // protected output streams
        ThreadSafeOutput consoleOut = ThreadSafeOutput(std::cout);
        ThreadSafeOutput consoleErr = ThreadSafeOutput(std::cerr);


        
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
        
        // handle incoming/outgoing messages
        // job: match messages to corresponding functions in simulator
        std::optional<MessageHandler> handler;

        bool running;

        void createMessageHandler();
        bool openBrowser(const std::string& url);

};
