#pragma once
#include "SimulationOptions.hpp"
#include "WebSocketServer.hpp"

#include <nlohmann/json.hpp>
#include <optional>
#include <thread>

class MissileDefenseSimulator {
    public:
        MissileDefenseSimulator();

        void createWebSocketServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port);
        void run();

    private:
        
        std::optional<SimulationOptions> options;      // house all options
        //SimulationWorld world;          // house true world state
        //ProcessManger manager;          // control child-process lifecycle
        //ComponentRegistry registry;     // house endpoints for components
        std::optional<WebSocketServer> server;         // front-end back-end communication
        //messageRouter router;           // route messages to components

        // queues for data transmisstion
        ThreadSafeQueue<nlohmann::json> webSocketIncoming;
        ThreadSafeQueue<nlohmann::json> webSocketOutGoing;

        // threads
        std::optional<std::thread> webServerThread;

        bool running;
};
