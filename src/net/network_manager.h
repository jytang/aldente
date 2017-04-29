#pragma once

#include "network_client.h"
#include "network_server.h"
#include "events.h"
#include <boost/thread.hpp>

#define CONN_RETRY_SEC 5.0f

class NetworkManager {
public:
    // Spawns a thread to periodically try connecting to server.
    static void connect(bool is_server);

    // Disconnects by stopping the io_service.
    static void disconnect();

    // Process input from server and client. Called in update loop.
    static void update();

    static NetworkServer* server;
    static NetworkClient* client;

private:
    // Periodically attempt connection every few seconds.
    static void attempt_connection();

    // Register event listeners.
    static void register_listeners();
    static void register_server_listeners();
    static void register_client_listeners();

    // Update server and client per game loop iteration.
    static void update_server();
    static void update_client();

    // Runs the io_service.
    static void run_service();

    static string server_host;
    static int port;
    static boost::thread* service_thread;
    static bool is_connected;
    static boost::asio::io_service io_service;
};
