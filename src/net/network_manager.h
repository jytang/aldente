#pragma once

#include "network_client.h"
#include "network_server.h"
#include "events.h"
#include <boost/thread.hpp>

#define CONN_RETRY_SEC 5.0f


class NetworkManager {
public:
    void disconnect();
    void run_service();
protected:
    boost::thread* service_thread;
    boost::asio::io_service io_service;
};

class ServerNetworkManager : public NetworkManager {
public:
    ServerNetworkManager() : server(io_service, 9000) {}
    void connect();
    void register_listeners();
    void update();
private:
    NetworkServer server;
};


class ClientNetworkManager : public NetworkManager {
public:
    ClientNetworkManager() : client(io_service) {}
    void connect();
    void register_listeners();
    void update();

private:
    void attempt_connection();

    NetworkClient client;
    string server_host;
    int port;
    bool is_connected;
};
