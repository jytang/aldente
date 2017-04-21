#pragma once
#include "net/NetworkClient.h"
#include "net/NetworkServer.h"
#include <boost/asio.hpp>

class NetworkManager {
public:
    NetworkManager();
    void connect();

    inline TcpServer* get_server() const { return server; };
    inline NetworkClient* get_client() const { return client; };

private:
    void attempt_connection();
    void NetworkManager::run_service();

    std::string server_host;
    bool is_server;
    int port;
    bool is_connected = false;
    TcpServer* server;
    NetworkClient* client;
    boost::asio::io_service io_service;
};