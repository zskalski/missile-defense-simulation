#pragma once
#include "SimulationOptions.hpp"
#include "WebSocketServer.hpp"
#include "HttpServer.hpp"
#include "ThreadSafeOutput.hpp"

#include <nlohmann/json.hpp>
#include <optional>
#include <thread>
#include <iostream>


class MissileDefenseSimulator {
    public:
        MissileDefenseSimulator();

        void createWebSocketServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port);
        void createHttpServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port);
        bool openBrowser(const std::string& url);
        void run();

    private:
        
        std::optional<SimulationOptions> options;      // house all options
        //SimulationWorld world;          // house true world state
        //ProcessManger manager;          // control child-process lifecycle
        //ComponentRegistry registry;     // house endpoints for components
        std::optional<WebSocketServer> webServer;           // front-end back-end communication
        std::optional<HttpServer> httpServer;               // front-end http get requests
        //messageRouter router;           // route messages to components

        // queues for data transmisstion
        ThreadSafeQueue<nlohmann::json> webSocketIncoming;
        ThreadSafeQueue<nlohmann::json> webSocketOutGoing;

        // protected output streams
        ThreadSafeOutput consoleOut = ThreadSafeOutput(std::cout);
        ThreadSafeOutput consoleErr = ThreadSafeOutput(std::cerr);


        // threads
        std::optional<std::thread> webServerThread;
        std::optional<std::thread> httpServerThread;

        bool running;
};
