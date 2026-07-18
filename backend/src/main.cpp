#include "MissileDefenseSimulator.hpp"

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define IP_ADDR "127.0.0.1"
#define HTTP_PORT 8080
#define WEBSOCKET_PORT 8081

int main() {

    // Simulation 
    auto simulation = MissileDefenseSimulator();

    // HTTP Server Parameters
    boost::asio::io_context ioContext;
    std::string ipAddr = IP_ADDR;
    unsigned short httpPort = HTTP_PORT;

    simulation.createHttpServer(ioContext, ipAddr, httpPort);

    // WebSocketServer Parameters
    unsigned short webPort = WEBSOCKET_PORT;
    
    simulation.createWebSocketServer(ioContext, ipAddr, webPort);

    simulation.run();

    return 0;
}