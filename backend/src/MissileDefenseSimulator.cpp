#pragma once

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

MissileDefenseSimulator::MissileDefenseSimulator()
    : webSocketIncoming(), webSocketOutGoing() {
}

void MissileDefenseSimulator::createWebSocketServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port) {
    // construct WebSocketServer in-place to avoid copying/moving
    webServer.emplace(ctx, address, port, webSocketIncoming, webSocketOutGoing, consoleOut, consoleErr);
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

    running = true;

    std::thread httpServerThread([this]() { httpServer->run(); });
    std::thread webServerThread([this]() { webServer->run(); });
    std::thread messageProcessingThread([this]() { handler.run(); });

    std::string url = "http://" + httpServer->getEndpoint().address().to_string() + ':' + std::to_string(httpServer->getEndpoint().port());
    openBrowser(url);

    messageProcessingThread.join();
    webServerThread.join();
    httpServerThread.join();
}