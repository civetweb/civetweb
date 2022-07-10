#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <unistd.h>
#include <unordered_map>

#include "CivetServer.h"

class WebSocketBase : public CivetWebSocketHandler
{

  public:
	explicit WebSocketBase(std::string name) : name_(std::move(name))
	{
	}

	bool
	handleConnection(CivetServer *server, const mg_connection *conn) override
	{
		return true;
	}

	void
	handleReadyState(CivetServer *server, mg_connection *conn) override
	{
		std::unique_lock<std::mutex> lock(mutex_);
		pool_.emplace(conn, std::make_shared<std::mutex>());
	}

	bool
	handleData(CivetServer *server,
	           mg_connection *conn,
	           int bits,
	           char *data,
	           size_t data_len) override
	{

		if ((bits & 0x0F) == MG_WEBSOCKET_OPCODE_CONNECTION_CLOSE) {
			return false;
		}
		data_.write(data, data_len);
		if (current_opcode_ == 0x00) {
			current_opcode_ = bits & 0x7f;
		}
		bool is_final_fragment = bits & 0x80;
		if (is_final_fragment) {
			switch (current_opcode_) {
			case MG_WEBSOCKET_OPCODE_TEXT:
				std::cout << data_.str() << std::endl; // print request data
				break;
			default:
				break;
			}
			current_opcode_ = 0x00;
			this->sendData(conn, data_.str()); // response
			data_.clear();
			data_.str(std::string());
		}
		return true;
	}

	void
	handleClose(CivetServer *server, const mg_connection *conn) override
	{
		auto *connection = const_cast<mg_connection *>(conn);
		std::shared_ptr<std::mutex> user_lock;
		{
			std::unique_lock<std::mutex> lock(mutex_);
			user_lock = pool_[connection];
		}
		{
			std::unique_lock<std::mutex> lock_connection(*user_lock);
			std::unique_lock<std::mutex> lock(mutex_);
			pool_.erase(connection);
		}
	}

	bool
	sendData(mg_connection *conn,
	         const std::string &data,
	         bool skippable = false,
	         int op_code = MG_WEBSOCKET_OPCODE_TEXT)
	{
		std::shared_ptr<std::mutex> connection_lock;
		{
			std::unique_lock<std::mutex> lock(mutex_);
			connection_lock = pool_[conn];
		}

		if (!connection_lock->try_lock()) {
			if (skippable) {
				return false;
			}
			connection_lock->lock();
			std::unique_lock<std::mutex> lock(mutex_);
		}

		int ret = mg_websocket_write(conn, op_code, data.c_str(), data.size());
		connection_lock->unlock();

		if (ret != static_cast<int>(data.size())) {
			if (data.empty() && ret == 2) {
				return true;
			}
			return false;
		}

		return true;
	}

	thread_local static std::stringstream data_;
	thread_local static unsigned char current_opcode_;

  private:
	const std::string name_;
	mutable std::mutex mutex_;
	std::unordered_map<mg_connection *, std::shared_ptr<std::mutex>> pool_;
};

thread_local unsigned char WebSocketBase::current_opcode_ = 0x00;
thread_local std::stringstream WebSocketBase::data_;

int
main()
{
	std::cout << "Hello, Civetweb Websocket port:8080!" << std::endl;

	mg_init_library(0);
	std::vector<std::string> options = {
	    "document_root",
	    ".",
	    "listening_ports",
	    "8080",
	    "access_control_allow_headers",
	    "*",
	    "access_control_allow_methods",
	    "*",
	    "access_control_allow_origin",
	    "*",
	};
	auto server = std::make_shared<CivetServer>(options);
	auto ws = std::make_shared<WebSocketBase>("simple");
	server->addWebSocketHandler("/ws", *ws);
	while (true) {
		sleep(1);
	}
	server->close();
	return 0;
}
