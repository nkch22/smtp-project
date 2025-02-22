#pragma once
#include <asio.hpp>

#include "ISocket.h"

namespace SMTP
{

class Socket : public ISocket
{
public:
	Socket();
	~Socket() = default;

	void Connect(const std::string& server, uint16_t port) override;
	void Disconnect() override;

	void Send(const std::string& message) override;

	SMTPResponse Receive() override;
	void AsyncReceive(ReceiveDelegate receiveDelegate) override;

private:
	void AsyncSendInternal(std::shared_ptr<std::string> buffer) override;

private:
	asio::io_context m_context;
	asio::ip::tcp::socket m_socket;
};

} // namespace SMTP