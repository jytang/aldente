#pragma once
#include "net/NetworkClient.h"
#include "net/NetworkServer.h"

class NetworkManager {
public:
    NetworkManager();
    void connect();

private:
    void attempt_connection();

    std::string server_host;
    bool is_server;
    bool is_connected = false;
    TcpServer* server;
    NetworkClient* client;
};