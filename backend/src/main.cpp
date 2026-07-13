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

    // Queues for Incoming/Outgoing Messages
    auto incomingMessages = ThreadSafeQueue<json>();
    auto outgoingMessages = ThreadSafeQueue<json>();

    // WebSocketServer Parameters
    boost::asio::io_context ioContext;
    std::string ipAddr = IP_ADDR;
    unsigned short port = PORT;

    auto server = WebSocketServer(ioContext, ipAddr, port, incomingMessages, outgoingMessages);

    // thread 1: run websocket server
    std::thread serverThread([&server]() {server.run();});

    // thread 2: run simulation
    // std::thread simulationThread(simulation.start());

    serverThread.join();
    // simulationThread.join();
    return 0;
}