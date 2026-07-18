#pragma once

#include "ThreadSafeOutput.hpp"

#include <boost/asio.hpp>
#include <filesystem>
#include <atomic>
#include <string>

class HttpServer {
    public:
        HttpServer(boost::asio::io_context & ctx, const std::string & address, unsigned short port, std::filesystem::path documentRoot, ThreadSafeOutput & outputStream, ThreadSafeOutput & errorStream);
        void run();
        void stop();

        boost::asio::ip::tcp::endpoint getEndpoint();

    private:
        void handleClient(boost::asio::ip::tcp::socket socket);

        std::string getMimeType(const std::filesystem::path& path);

        boost::asio::io_context & ioContext;
        boost::asio::ip::tcp::endpoint serverEndpoint;
        boost::asio::ip::tcp::acceptor acceptor;

        std::filesystem::path documentRoot;
        std::atomic_bool running;

        ThreadSafeOutput & consoleOut;
        ThreadSafeOutput & consoleErr;
};