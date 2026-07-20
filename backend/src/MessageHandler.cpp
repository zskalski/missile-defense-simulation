#include "MessageHandler.hpp"
#include "ThreadSafeOutput.hpp"
#include "ThreadSafeQueue.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <any>

using json = nlohmann::json;

MessageHandler::MessageHandler(ThreadSafeOutput & out, ThreadSafeOutput & error, ThreadSafeQueue<json> & inMessages, ThreadSafeQueue<json> & outMessages) : consoleOut(out), consoleErr(error), incomingMessages(inMessages), outGoingMessages(outMessages) {

    // populate each map with responses/requests
    
    // all responses to messages will be here:
    messageResponses = {
        // lamba function parts:
        // [capture] (parameters) {body}
        // "this" is needed to capture the MessageHandler instance, since sendUpdateJson is a member function and not static
        {"update.request", [this] (const json& message) {sendUpdateJson(message);}}
    };

    // all types of messages requests stemming from the backend will be here:
}

void MessageHandler::run() {
    running.store(true);

    consoleOut.write("Backend message handler started.\n");

    while(running.load()) {
        consoleOut.write("[MessageHandler] Attempting to pop message from Queue\n");
        auto message = incomingMessages.popOrWait();
        consoleOut.write("[messageHandler] Processing message: " + message->dump() + '\n');
        processMessage(message);
    }

    consoleOut.write("Backend message handler stopped.\n");

    return;
}

void MessageHandler::processMessage(const json & message) {
    try {
        const std::string type = message.at("type").get<std::string>();
        const auto handler = messageResponses.find(type);

        if (handler == messageResponses.end()) {
            consoleErr.write("processMessage Error: message type not recognized: ", type, '\n');
            return;
        }
        consoleOut.write("[MessageHandler -> processMessage()]: attempting to call message handler.\n");
        // second part of the handler is the function
        handler->second(message);
    } 
    catch (const std::exception& e){
        consoleErr.write("processMessage Error: ", e.what(), '\n');
    }
}

nlohmann::json MessageHandler::getJsonRequest(const std::string & type) {
    return {"type", "null"};
}

void MessageHandler::sendUpdateJson(const json& message) {
    json updateJson = {
            {"type", "update.response"},
            {"payload", {
                {"hours", 1},
                {"minutes", 21},
                {"seconds", 55}
            }}
        };
    outGoingMessages.push(updateJson);
}