#include "NetworkServer.h"

TcpConnection::pointer TcpConnection::create(boost::asio::io_service& io_service) {
  return pointer(new TcpConnection(io_service));
}

tcp::socket& TcpConnection::get_socket() {
  return socket;
}

void TcpConnection::start() {
  socket.async_read_some(boost::asio::buffer(rcvbuf),
      boost::bind(&TcpConnection::handle_read, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

bool TcpConnection::send(std::string message) {
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

bool TcpConnection::has_messages() {
  return !message_queue.empty();
}

std::string TcpConnection::pop_message() {
  if (!has_messages()) 
    throw std::logic_error("No messages"); 
  return message_queue.pop(); 
}

TcpConnection::TcpConnection(boost::asio::io_service& io_service)
  : socket(io_service) {}

void TcpConnection::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {    
  if (!error) {
    message_queue.push(rcvbuf);
  }
  else if (error != boost::asio::error::eof) {
    std::cerr << "FATAL handle_read error: " << error << "\n";
    return;
  }

  // Wait for and read the next message.
  socket.async_read_some(boost::asio::buffer(rcvbuf),
      boost::bind(&TcpConnection::handle_read, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

TcpServer::TcpServer(unsigned int port) :
  acceptor_(io_service, tcp::endpoint(tcp::v4(), port)), next_id(0) {
  start_accept();
  
  // Run the io_service in a separate thread so it's non-blocking.
  std::thread(TcpServer::run, std::ref(io_service)).detach();
}

void TcpServer::send_to_all(std::string message) {
  // No clients connected.
  if (client_list.size() == 0) {
    return;
  }
  
  std::cerr << "[server] writing to clients, connected: " << client_list.size() << "\n";
  for (auto c = client_list.begin(); c != client_list.end(); /* empty */) {
	  bool success = c->second->send(message);
	  if (!success) {
		  std::cerr << "Write failed!\n";
		  c = client_list.erase(c);
	  }
	  else {
		  c++;
	  }
  }
}

std::vector<std::string> TcpServer::read_all_messages() {
  std::vector<std::string> messages;
  
  // No clients connected.
  if (client_list.size() == 0) {
    return messages;
  }
  
  for (auto const &c : client_list) {
    while (c.second->has_messages()) {
      auto message = c.second->pop_message();
      messages.push_back(message);
    }
  }
  
  return messages;
}

void TcpServer::start_accept() {
  TcpConnection::pointer new_connection =
    TcpConnection::create(acceptor_.get_io_service());

  acceptor_.async_accept(new_connection->get_socket(),
      boost::bind(&TcpServer::handle_accept, this, new_connection,
        boost::asio::placeholders::error));
}

void TcpServer::handle_accept(TcpConnection::pointer new_connection,
    const boost::system::error_code& error) {
  if (!error) {
    std::cerr << "Accepted new connection." << std::endl;
    client_list[++next_id] = new_connection;
    new_connection->start();
  }
  else {
	  std::cerr << "accept() error: " << error << "," << error.message() << "\n";
  }

  // Accept the next client.
  start_accept();
}

void TcpServer::run(boost::asio::io_service& io_service) {
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