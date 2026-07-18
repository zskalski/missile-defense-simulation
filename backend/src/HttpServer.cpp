#include "HttpServer.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <boost/asio/ip/address.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <filesystem>
#include <syncstream>

HttpServer::HttpServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port, std::filesystem::path documentRoot, ThreadSafeOutput & outputStream, ThreadSafeOutput & errorStream)
:   ioContext(ctx), 
    serverEndpoint(boost::asio::ip::make_address(address), port), 
    acceptor(ioContext, serverEndpoint), 
    documentRoot(std::move((documentRoot))), 
    running(false),
    consoleOut(outputStream),
    consoleErr(errorStream)
{}

void HttpServer::run() {
    running.store(true);

    consoleOut.write(
        "HTTP Server listening at http://", 
        serverEndpoint.address(), 
        ':', 
        serverEndpoint.port(),
        "\nOpen http://",
        serverEndpoint.address(), 
        ':', 
        serverEndpoint.port(),
        " in a browser if it has not already popped up.",
        '\n'
    );

    while(running.load()) {
        boost::beast::error_code error;
        boost::asio::ip::tcp::socket socket(ioContext);
        acceptor.accept(socket, error);
        if (error) {
            if(!running.load()) {
                break;
            }
            consoleErr.write("HTTP accept error: ", error.message(), '\n');
            continue;
        }

        handleClient(std::move(socket));
    }
}

void HttpServer::stop() {
    running.store(false);

    boost::beast::error_code error;
    acceptor.close(error);

    if (error) {
        consoleErr.write("HTTP server stop error: ", error.message(), '\n');
    }
}

void HttpServer::handleClient(boost::asio::ip::tcp::socket socket) {

    boost::beast::flat_buffer buffer;
    boost::beast::error_code error;

    // request ex. GET /index.html HTTP/1.1
    boost::beast::http::request<boost::beast::http::string_body> request;

    // Read the http request on the socket, store raw data buffer and parse to request variable
    boost::beast::http::read(socket, buffer, request, error);

    if (error) {
        consoleErr.write("HTTP read error: ", error.message(), '\n');
        return;
    }

    // Reject any requests that are not GET
    if (request.method() != boost::beast::http::verb::get) {
        boost::beast::http::response<boost::beast::http::string_body> response {
            boost::beast::http::status::method_not_allowed,
            request.version()
        };

        response.set(
            boost::beast::http::field::content_type,
            "text/plain"
        );

        response.body() = "Only GET requests are supported.";
        response.prepare_payload();

        boost::beast::http::write(socket, response, error);
        return;
    }

    // target is the filepath to what the GET wants
    std::string target(request.target());

    if (target == "/") {
        target = "/index.html";
    }

    // remove leading '/'
    std::filesystem::path relativePath = target.substr(1);

    // '/' appends one path to another, so the filename looks like "frontend/index.html"
    std::filesystem::path requestedPath = documentRoot / relativePath;

    // rejects directories or paths that dont exist
    if (!std::filesystem::exists(requestedPath) ||
        !std::filesystem::is_regular_file(requestedPath)) {

        boost::beast::http::response<boost::beast::http::string_body> response{
            boost::beast::http::status::not_found,
            request.version()
        };

        response.set(
            boost::beast::http::field::server,
            "MissileDefenseSimulator"
        );

        response.set(
            boost::beast::http::field::content_type,
            "text/plain"
        );

        response.body() = "File not found.";
        response.prepare_payload();

        boost::beast::http::write(socket, response, error);
        return;
    }

    // create a response that has the contents of the file
    boost::beast::http::response<boost::beast::http::file_body> response{
        boost::beast::http::status::ok,
        request.version()
    };

    response.body().open(
        requestedPath.string().c_str(),
        boost::beast::file_mode::scan,
        error
    );

    if (error) {
        consoleErr.write("Could not open file: ", requestedPath, '\n');
        return;
    }

    response.set(
        boost::beast::http::field::server,
        "MissileDefenseSimulator"
    );

    response.set(
        boost::beast::http::field::content_type,
        getMimeType(requestedPath)
    );

    response.content_length(response.body().size());
    response.keep_alive(false);

    // write the response to the frontend
    boost::beast::http::write(socket, response, error);

    if (error) {
        consoleErr.write("HTTP write error: ", error.message(), '\n');
    }

    socket.shutdown(
        boost::asio::ip::tcp::socket::shutdown_send,
        error
    );

    if (error) {
        consoleErr.write("Socket shutdown error: ", error.message(), '\n');
    }
}

std::string HttpServer::getMimeType(const std::filesystem::path& path) {

    const std::string extension = path.extension().string();

    if (extension == ".html") {
        return "text/html";
    }

    if (extension == ".css") {
        return "text/css";
    }

    if (extension == ".js") {
        return "text/javascript";
    }

    if (extension == ".json") {
        return "application/json";
    }

    if (extension == ".png") {
        return "image/png";
    }

    if (extension == ".jpg" || extension == ".jpeg") {
        return "image/jpeg";
    }

    if (extension == ".svg") {
        return "image/svg+xml";
    }

    if (extension == ".ico") {
        return "image/x-icon";
    }

    if (extension == ".pdf") {
        return "application/pdf";
    }

    return "application/octet-stream";
}

boost::asio::ip::tcp::endpoint HttpServer::getEndpoint() {
    return serverEndpoint;
}