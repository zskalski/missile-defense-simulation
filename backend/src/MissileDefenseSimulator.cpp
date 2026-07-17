#pragma once

#include "MissileDefenseSimulator.hpp"
#include "ThreadSafeQueue.hpp"
#include "WebSocketServer.hpp"

#include <nlohmann/json.hpp>
#include <thread>
#include <iostream>

MissileDefenseSimulator::MissileDefenseSimulator()
    : webSocketIncoming(), webSocketOutGoing() {
}

void MissileDefenseSimulator::createWebSocketServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port) {
    // construct WebSocketServer in-place to avoid copying/moving
    server.emplace(ctx, address, port, webSocketIncoming, webSocketOutGoing);
}

void MissileDefenseSimulator::run() {
    running = true;
    std::thread webServerThread([this]() { server->run(); });

    webServerThread.join();
}