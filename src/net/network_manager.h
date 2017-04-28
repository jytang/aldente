#pragma once

#include "network_client.h"
#include "network_server.h"
#include <boost/thread.hpp>

#define CONN_RETRY_SEC 5.0f

class NetworkManager {
public:
    // Spawns a thread to periodically try connecting to server.
    static void connect();

    // Disconnects by stopping the io_service.
    static void disconnect();

    // Process input from server and client. Called in update loop.
    static void update();

    static NetworkServer* server;
    static NetworkClient* client;

private:
    // Periodically attempt connection every few seconds.
    static void attempt_connection();

    // Runs the io_service.
    static void run_service();

    static bool is_server;
    static string server_host;
    static int port;
    static boost::thread* service_thread;
    static bool is_connected;
    static boost::asio::io_service io_service;
};
