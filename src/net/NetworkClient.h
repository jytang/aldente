/**
* Source: http://codereview.stackexchange.com/questions/51235/udp-network-server-client-for-gaming-using-boost-asio
* License: MIT
*/

#include "threadsafe_queue.h"
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <array>

#define PORT "9000"

using boost::asio::ip::tcp;

/**
* Represents a single client on the network.
*/
class NetworkClient {
public:
  NetworkClient(std::string host);

  // Destructor.
  ~NetworkClient();

  // Synchronously tries to connect to host. Returns true if successful.
  bool init();

  // Returns true if client was successfully initialized.
  bool is_initialized() const;

  // Synchronously sends a message to the server. 
  void send(std::string message);

  // Returns true if there are message(s) in the queue.
  bool has_messages();

  // Removes and returns a message from the FIFO queue.
  std::string pop_message();

private:
  // Begin receiving messages by adding an async receive task.
  void start_receive();

  // Callback for when receive is completed. Adds to message queue, continue reading.
  void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred);

  // Service thread for receiving messages.
  void run_service();

  boost::asio::io_service io_service;
  tcp::socket socket;
  boost::array<char, BUFSIZ> recv_buffer;
  boost::thread service_thread;
  ThreadSafeQueue<std::string> message_queue;
  std::string host;
  bool initialized;
};