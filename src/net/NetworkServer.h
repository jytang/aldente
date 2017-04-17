#include "threadsafe_queue.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>

using boost::asio::ip::tcp;

/**
 * Represents one TCP connection to a client.
 */
class TcpConnection : public boost::enable_shared_from_this<TcpConnection> {
public:
  typedef boost::shared_ptr<TcpConnection> pointer;

  // Create a shared pointer to this TCP connection.
  static pointer create(boost::asio::io_service& io_service);

  // Returns this connection's socket.
  tcp::socket& get_socket();

  // When connection starts, begin reading.
  void start();
  
  // Sends a message to this client. Returns true if write was successful.
  bool send(std::string message);
  
  // Returns true if there are messages in the queue.
  bool has_messages();
  
  // Removes a message and returns it.
  std::string pop_message();

private:
  // Initializes the socket.
  TcpConnection(boost::asio::io_service& io_service);
  
  // Callback for when an asynchronous  read completes. 
  void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

  tcp::socket socket;
  char rcvbuf[BUFSIZ];
  ThreadSafeQueue<std::string> message_queue;
};

/**
 * Represents the single TCP server, managing many client connections.
 */
class TcpServer {
public:
  // Initializes this server with the given port.
  TcpServer(unsigned int port);
  
  // Sends a message to all clients.
  void send_to_all(std::string message);
  
  // Read all messages from all clients.
  std::vector<std::string> read_all_messages();

private:
  // Begin accepting new clients.
  void start_accept();

  // Callback for when a client is connected.
  void handle_accept(TcpConnection::pointer new_connection,
      const boost::system::error_code& error);

  // Run the io_service. Is run on a separate thread to avoid blocking.
  static void run(boost::asio::io_service& io_service);
  
  boost::asio::io_service io_service;
  tcp::acceptor acceptor_;
  std::map<int, TcpConnection::pointer> client_list;
  int next_id;
};
