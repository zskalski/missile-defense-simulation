#pragma once

#include <atomic>
#include <functional>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "SimulationWorld.hpp"
#include "ThreadSafeOutput.hpp"
#include "ThreadSafeQueue.hpp"

using json = nlohmann::json;

class MessageHandler {
    public:

        using MessageCallback = std::function<void(const json&)>;

        MessageHandler(
            ThreadSafeOutput & output, 
            ThreadSafeOutput & error, 
            ThreadSafeQueue<json> & inMessages,
            MessageCallback sendUpdateCallback,
            MessageCallback startCallback,
            MessageCallback pauseCallback,
            MessageCallback resetCallback,
            MessageCallback doAutoCallback,
            MessageCallback radarVisCallback,
            MessageCallback simSpeedCallback,
            MessageCallback placementCallback 
        );
        
        void run();
        void processMessage(const json &);
        json getJsonRequest(const std::string &);

    private:

        std::atomic_bool running;

        // output streams
        ThreadSafeOutput & consoleOut;
        ThreadSafeOutput & consoleErr;

        ThreadSafeQueue<json> & incomingMessages;

        // storing functions that handle incoming messages
        std::unordered_map<std::string, MessageCallback> messageResponses;

        // storing requests that backend can send
        std::unordered_map<std::string, json> messageRequests;



        // Message handlers ------------------
        
        // state updates
        MessageCallback sendUpdateCallback;
        
        // simulation control
        MessageCallback startCallback;
        MessageCallback pauseCallback;
        MessageCallback resetCallback;

        // user options
        MessageCallback doAutoCallback;
        MessageCallback radarVisCallback;
        MessageCallback simSpeedCallback;
        MessageCallback placementCallback;
};