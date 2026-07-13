#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp>
#include "ThreadSafeQueue.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <memory>
#include <optional>

class WebSocketServer {

    public:
        using tcp = boost::asio::ip::tcp;
        using json = nlohmann::json;
        using WebSocket = boost::beast::websocket::stream<tcp::socket>;

        WebSocketServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port, ThreadSafeQueue<json>& in, ThreadSafeQueue<json>& out);

        int run();

        enum class ReadResult {
            NormalRequest,
            ShutdownRequest,
            InvalidMessage
            
        };
        

    private:
        boost::asio::io_context & ioContext;        // context to manage i/o, OS link
        tcp::endpoint serverEndpoint;               // access to address , port, protocol
        tcp::acceptor acceptor;                     // acceptor binds to endpoint
        std::optional<WebSocket> webSocketStream;                     
        ThreadSafeQueue<json>& incomingMessages;
        ThreadSafeQueue<json>& outgoingMessages;
        
        bool running;

        void acceptClient();
        void handleClient();
        ReadResult readMessage();
        bool sendMessage();
        json processMessage(const std::string& message);
        void sendDirectMessage(const json& message);
};

/*
    JSON MESSAGE STRUCUTURE
    {
        "type": "object.created"        (type of the message )
        "payload:" {                    (specific information being sent)
            "object": {
                "id": "radar-14",
                "objectType": "radar",
                "position": {
                    "row": 5,
                    "column": 8
                },
                "range": 300,
                "active": true
            }
        }
    }
*/