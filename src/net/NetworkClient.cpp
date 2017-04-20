#include "NetworkClient.h"

NetworkClient::NetworkClient(std::string host) :
	socket(io_service), host(host) {
}

NetworkClient::~NetworkClient() {
	io_service.stop();
	service_thread.join();
}

bool NetworkClient::init() {
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(tcp::v4(), host, PORT);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	try {
		boost::asio::connect(socket, endpoint_iterator);
	}
	catch (...) {
		return false;
	}
	service_thread = boost::thread(boost::bind(&NetworkClient::run_service, this));
	initialized = true;
	return true;
}

bool NetworkClient::is_initialized() const {
	return initialized;
}

void NetworkClient::send(std::string message) {
	boost::asio::write(socket, boost::asio::buffer(message));
}

bool NetworkClient::has_messages() {
	return !message_queue.empty();
}

std::string NetworkClient::pop_message() {
	if (!has_messages())
		throw std::logic_error("No messages");
	return message_queue.pop();
}

void NetworkClient::start_receive() {
	socket.async_receive(boost::asio::buffer(recv_buffer),
		boost::bind(&NetworkClient::handle_receive, this,
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void NetworkClient::handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
	if (!error) {
		std::string message(recv_buffer.data(), recv_buffer.data() + bytes_transferred);
		message_queue.push(message);
	}

	start_receive();
}

void NetworkClient::run_service() {
	start_receive();
	while (!io_service.stopped()) {
		try {
			io_service.run();
		}
		catch (const std::exception& e) {
			std::cerr << "Client network exception" << e.what();
		}
		catch (...) {
			std::cerr << "Unknown exception in client network";
		}
	}
}