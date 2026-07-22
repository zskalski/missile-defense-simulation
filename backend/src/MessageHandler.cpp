#include "MessageHandler.hpp"
#include "ThreadSafeOutput.hpp"
#include "ThreadSafeQueue.hpp"
#include "SimulationWorld.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <any>


using json = nlohmann::json;

MessageHandler::MessageHandler(
    ThreadSafeOutput & out, 
    ThreadSafeOutput & error,
    ThreadSafeQueue<json> & inMessages,
    MessageCallback sendUpdateCallback,
    MessageCallback startCallback,
    MessageCallback pauseCallback,
    MessageCallback resetCallback,
    MessageCallback doAutoCallback,
    MessageCallback radarVisCallback,
    MessageCallback simSpeedCallback) 
    :   consoleOut(out), 
        consoleErr(error), 
        incomingMessages(inMessages),
        sendUpdateCallback(sendUpdateCallback),
        startCallback(startCallback),
        pauseCallback(pauseCallback),
        resetCallback(resetCallback),
        doAutoCallback(doAutoCallback),
        radarVisCallback(radarVisCallback),
        simSpeedCallback(simSpeedCallback) {

    running.store(false);

    // populate each map with responses/requests
    
    // all responses to messages will be here:
    messageResponses = {
        // lamba function parts:
        // [capture] (parameters) {body}
        // "this" is needed to capture the MessageHandler instance, which is then used to access the callback function variable
        {"update.request", this->sendUpdateCallback},
        {"start.request", this->startCallback},
        {"pause.request", this->pauseCallback},
        {"reset.request", this->resetCallback},
        {"doAuto.request", this->doAutoCallback},
        {"radarVis.request", this->radarVisCallback},
        {"simSpeed.request", this->simSpeedCallback}
    };

    // all types of messages requests stemming from the backend will be here:
    messageRequests = {

    };
}

void MessageHandler::run() {
    running.store(true);

    consoleOut.write("Backend message handler started.\n");

    while(running.load()) {
        //consoleOut.write("[MessageHandler] Attempting to pop message from Queue\n");
        auto message = incomingMessages.popOrWait();

        // pop or wait returns null if queue is closed && empty
        if (!message.has_value()) {
            break;
        }

        //consoleOut.write("[messageHandler] Processing message: " + message->dump() + '\n');
        // dereference as process message expects json, not optional<json>
        processMessage(*message);
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
        //consoleOut.write("[MessageHandler -> processMessage()]: attempting to call message handler.\n");
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
