#include "WebSocketServer.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include <iostream>
#include <string>
#include <utility>
#include <stdexcept>

namespace ip = boost::asio::ip;

using tcp = boost::asio::ip::tcp;
using json = nlohmann::json;
using WebSocket = boost::beast::websocket::stream<tcp::socket>;

WebSocketServer::WebSocketServer(boost::asio::io_context & ctx, const std::string & addr, unsigned short port, ThreadSafeQueue<json>& in, ThreadSafeQueue<json>& out, ThreadSafeOutput & outputStream, ThreadSafeOutput & errorStream):
    ioContext(ctx), 
    serverEndpoint(tcp::endpoint(ip::make_address(addr), port)),
    acceptor(ioContext, serverEndpoint), 
    running(false), 
    webSocketStream(std::nullopt),
    incomingMessages(in),
    outgoingMessages(out), 
    consoleOut(outputStream),
    consoleErr(errorStream) {
}

int WebSocketServer::run() {
    running = true;

    consoleOut.write(
        "WebSocket server listening on ws://",
        serverEndpoint.address(),
        ':',
        serverEndpoint.port(),
        '\n'
    );

    while (running) {
        try {
            consoleOut.write("going into accept client()\n");
            acceptClient();
            consoleOut.write("going into handle client()\n");
            handleClient();
        }
        catch (const boost::system::system_error& error) {
            webSocketStream.reset();

            if (error.code() == boost::beast::websocket::error::closed) {
                consoleErr.write("Client disconnected\n");
            }
            else {
                consoleErr.write("Network error: ", error.code().message(), '\n');
            }
        }
        catch (const std::exception& error) {
            webSocketStream.reset();
            consoleErr.write("Server error: ", error.what(), '\n');
        }
        
    }

    // attempt to close websocket
    boost::system::error_code error;
    webSocketStream->close(boost::beast::websocket::close_code::normal,error);

    if (error && error != boost::beast::websocket::error::closed) {
        consoleErr.write("WebSocket close error: ", error.message(), '\n');
    }

    // attempt to close TCP connection
    if (webSocketStream && webSocketStream->next_layer().is_open()) {
        boost::system::error_code error;
        webSocketStream->next_layer().cancel(error);
        if(error) 
            consoleErr.write("Error canceling TCP connection: ", error.message(), '\n');

        error.clear();
        webSocketStream->next_layer().close(error);

        if(error) 
            consoleErr.write("Error closing TCP connection: ", error.message(), '\n');
    }

    consoleOut.write("WebSocket server stopped.\n");
    return EXIT_SUCCESS;
}


void WebSocketServer::acceptClient() {
    
    consoleOut.write("Waiting for client...\n");
    
    boost::system::error_code error;

    tcp::socket socket(ioContext);
    acceptor.accept(socket, error);

    if (error) {
        throw boost::system::system_error(error);
    }
    
    consoleOut.write("TCP client connected.\n");
    
    webSocketStream.emplace(std::move(socket));
    webSocketStream->accept(error);

    if (error) {
        throw boost::system::system_error(error);
    }

    consoleOut.write("WebSocket handshake complete.\n");

    // send ready message to the frontend
    json message  = {{"type", "ready"}};
    sendDirectMessage(message);
}

void WebSocketServer::handleClient() {
    while (running && webSocketStream) {
        consoleOut.write("Inside of handleclient(), attempting to read message\n");
        ReadResult result = readMessage();
        if (result == ReadResult::InvalidMessage)
            continue;           // skip sending message if no message is read
        if (result == ReadResult::ShutdownRequest) {
            running = false;
            return;
        }
        if (result == ReadResult::NormalRequest)
            consoleOut.write("Recieved normal request, attempting to sendMessage()\n");
            // if false from sendMessage, means the simulation thread has shutdown the queue
            if (!sendMessage()) {
                running = false;
                return;
            }
    }
}



WebSocketServer::ReadResult WebSocketServer::readMessage() {

    boost::beast::flat_buffer buffer;
    webSocketStream->read(buffer);
    const std::string messageText = boost::beast::buffers_to_string(buffer.data());
    consoleOut.write("Raw message: ", messageText, '\n');

    try {
        json message = processMessage(messageText);
        consoleOut.write("[readMessage]: Pushing message into incomingMessagesQueue\n");
        incomingMessages.push(message);

        if (message.at("type") == "shutdown") {
            consoleOut.write("Shutdown messaged recieved.\n");
            return ReadResult::ShutdownRequest;
        }
        consoleOut.write( "Message: ", message, "\nSuccessfully added to the queue.\n");
    }
    catch (const json::exception & error) {
        consoleErr.write("JSON error: ", error.what(), '\n');

        // send an error message back to the client
        json message  = {
            {"type", "error.json"},
            {"payload", {
                {"code", error.id},
                {"message", error.what()}
            }}
        };
        sendDirectMessage(message);

        return ReadResult::InvalidMessage;
    }

    return ReadResult::NormalRequest;
}

bool WebSocketServer::sendMessage() {
    
    // wait for a message to appear in the queue
    std::optional<json> message = outgoingMessages.popOrWait();

    // represents a server-side shutdown enabled by the queue closing
    if (!message) {
        return false;
    }

    const std::string messageText = message->dump();

    webSocketStream->text(true);
    webSocketStream->write(boost::asio::buffer(messageText));

    return true;
}

json WebSocketServer::processMessage(const std::string& message) {
    const json parsedMessage = json::parse(message);
    consoleOut.write("Parsed message: ", parsedMessage, '\n');
    return parsedMessage;
}

void WebSocketServer::sendDirectMessage(const json& message) {
    const std::string messageText = message.dump();
    webSocketStream->text(true);
    webSocketStream->write(boost::asio::buffer(messageText));
}

tcp::endpoint WebSocketServer::getEndpoint() {
    return serverEndpoint;
}