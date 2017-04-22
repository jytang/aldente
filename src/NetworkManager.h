#pragma once
#include "net/NetworkClient.h"
#include "net/NetworkServer.h"
#include <boost/asio.hpp>

#define CONN_RETRY_SEC 5.0f

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    // Spawns a thread to periodically try connecting to server.
    void connect();

    // Disconnects by stopping the io_service.
    void disconnect();

    inline NetworkServer* get_server() const { return server; };
    inline NetworkClient* get_client() const { return client; };

private:
    // Periodically attempt connection every few seconds.
    void attempt_connection();

    // Runs the io_service.
    void run_service();

    std::string server_host;
    boost::thread* service_thread;
    bool is_server;
    int port;
    bool is_connected = false;
    NetworkServer* server;
    NetworkClient* client;
    boost::asio::io_service io_service;
};
