#pragma once
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>
#include <atomic>
#include "ThreadSafeOutput.hpp"
#include "ThreadSafeQueue.hpp"

using json = nlohmann::json;

class MessageHandler {
    public:
        MessageHandler(ThreadSafeOutput & output, ThreadSafeOutput & error, ThreadSafeQueue<json> & incomingMessages, ThreadSafeQueue<json> & outGoingMessages);
        void run();
        void processMessage(const json &);
        json getJsonRequest(const std::string &);
    private:

        std::atomic_bool running;

        // output streams
        ThreadSafeOutput & consoleOut;
        ThreadSafeOutput & consoleErr;

        // input / output command queue
        ThreadSafeQueue<json> & incomingMessages;
        ThreadSafeQueue<json> & outGoingMessages;

        using MessageFunction = std::function<void(const json&)>;

        // storing functions that handle incoming messages
        std::unordered_map<std::string, MessageFunction> messageResponses;

        // storing requests that backend can send
        std::unordered_map<std::string, json> messageRequests;

        // message handlers
        void sendUpdateJson(const json& message);
};