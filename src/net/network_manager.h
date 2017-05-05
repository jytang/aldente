#pragma once

#include "network_client.h"
#include "network_server.h"
#include "events.h"
#include <boost/thread.hpp>

#define CONN_RETRY_SEC 5.0f

/*
class NetworkManager {
public:
    virtual void connect() = 0;
    virtual void update() = 0;
    void disconnect();
    //void run_service();

protected:
    virtual void register_listeners() = 0;

    boost::thread* service_thread;
    boost::asio::io_service io_service;
};*/

class ServerNetworkManager {
public:
    ServerNetworkManager() : server(io_service, 9000) {}
    void connect();
    void register_listeners();
    void update();
    void run_service();
private:
    NetworkServer server;
    boost::thread* service_thread;
    boost::asio::io_service io_service;
};


class ClientNetworkManager {
public:
    void connect();
    void register_listeners();
    void update();
    void run_service();

private:
    void attempt_connection();

    NetworkClient client;
    string server_host;
    int port;
    bool is_connected;
    boost::thread* service_thread;
    boost::asio::io_service io_service;
};
