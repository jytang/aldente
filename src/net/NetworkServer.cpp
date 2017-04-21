#include "NetworkServer.h"

Connection::Connection(boost::asio::io_service& io_service)
    : socket(io_service) {}

tcp::socket& Connection::get_socket() {
  return socket;
}

void Connection::read_loop() {
  socket.async_read_some(boost::asio::buffer(rcvbuf),
      boost::bind(&Connection::handle_read, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

bool Connection::send(std::string message) {
  if (!socket.is_open()) {
    return false;
  }
  
  boost::system::error_code error;
  boost::asio::write(socket, boost::asio::buffer(message), error);
  
  // If we get these errors, it's likely a clean disconnect.
  if ((error == boost::asio::error::eof) ||
      (error == boost::asio::error::connection_reset) ||
      (error == boost::asio::error::broken_pipe)) {
    std::cerr << "[send] client disconnected.\n";
    return false;
  } else if (error) {
    std::cerr << "[send] some other error: " << error << "\n";
    return false;
  }
  
  return true;
}

bool Connection::read_message(std::string& message) {
    if (message_queue.empty())
        return false;
    message = message_queue.pop();
    return true;
}

void Connection::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {    
  if (!error) {
    message_queue.push(rcvbuf);
    std::cerr << "[s] recv: " << std::string(rcvbuf);
  }
  else if (error != boost::asio::error::eof) {
    std::cerr << "FATAL handle_read error: " << error << "\n";
    return;
  }

  // Wait for and read the next message.
  read_loop();
}

NetworkServer::NetworkServer(unsigned int port) :
  acceptor(io_service, tcp::endpoint(tcp::v4(), port)), next_id(0) {
  start_accept();
  
  // Run the io_service in a separate thread so it's non-blocking.
  std::thread(NetworkServer::run, std::ref(io_service)).detach();
}

void NetworkServer::send_to_all(std::string message) {
  unique_lock<mutex> lock(client_list_mutex);

  // No clients connected.
  if (client_list.empty()) {
    return;
  }
  
  for (auto c = client_list.begin(); c != client_list.end(); /* empty */) {
    bool success = c->second->send(message);
    if (!success) {
      std::cerr << "Write failed!\n";
      // If write failed, it's likely because of disconnect. Remove from clients.
      c = client_list.erase(c);
    }
    else {
      c++;
    }
  }
}

std::map<int, std::vector<std::string>> NetworkServer::read_all_messages() {
  std::map<int, std::vector<std::string>> messages;
  unique_lock<mutex> lock(client_list_mutex);
  
  // No clients connected.
  if (client_list.empty()) {
    return messages;
  }
  
  for (auto const &c : client_list) {
    messages[c.first] = std::vector<std::string>();
    std::string message;
    while (c.second->read_message(message)) {
        messages[c.first].push_back(message);
    }
  }
  
  return messages;
}

void NetworkServer::start_accept() {
  Connection::pointer new_connection =
    Connection::pointer(new Connection(acceptor.get_io_service()));

  acceptor.async_accept(new_connection->get_socket(),
      boost::bind(&NetworkServer::handle_accept, this, new_connection,
        boost::asio::placeholders::error));
}

void NetworkServer::handle_accept(Connection::pointer new_connection,
    const boost::system::error_code& error) {
  if (!error) {
    std::cerr << "Accepted new connection." << std::endl;
    unique_lock<mutex> lock(client_list_mutex);
    client_list[++next_id] = new_connection;
    lock.unlock();
    new_connection->read_loop();
  }
  else {
      std::cerr << "accept() error: " << error << "," << error.message() << "\n";
  }

  // Accept the next client.
  start_accept();
}

void NetworkServer::run(boost::asio::io_service& io_service) {
    while (!io_service.stopped()) {
        try {
            io_service.run();
        }
        catch (std::exception const&  ex) {
            std::cerr << "io_service except: " << ex.what() << "\n";
        }
        catch (...) {
            std::cerr << "io_service error\n";
        } // temporary ignore
    }
}