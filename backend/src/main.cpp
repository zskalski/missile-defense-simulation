#include "ThreadSafeQueue.hpp"
#include "WebSocketServer.hpp"
#include "MissileDefenseSimulator.hpp"
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <thread>
#include <vector>

using json = nlohmann::json;

#define IP_ADDR "127.0.0.1"
#define PORT 8080

int main() {

    // Simulation 
    auto simulation = MissileDefenseSimulator();

    // WebSocketServer Parameters
    boost::asio::io_context ioContext;
    std::string ipAddr = IP_ADDR;
    unsigned short port = PORT;
    
    simulation.createWebSocketServer(ioContext, ipAddr, port);

    simulation.run();

    return 0;
}