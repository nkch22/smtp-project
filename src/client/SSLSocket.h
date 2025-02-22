#pragma once
#include <asio.hpp>
#include <asio/ssl.hpp>

#include "ISocket.h"

namespace SMTP
{

class SSLSocket : public ISocket
{
public:
	typedef asio::ssl::stream<asio::ip::tcp::socket> ssl_socket;

public:
	SSLSocket();
	~SSLSocket() = default;

	void Connect(const std::string& server, uint16_t port) override;
	void Disconnect() override;

	void Send(const std::string& message) override;

	SMTPResponse Receive() override;
	void AsyncReceive(ReceiveDelegate receiveDelegate) override;

private:
	void AsyncSendInternal(std::shared_ptr<std::string> buffer) override;
	void HandShake();

private:
	asio::io_context m_context;
	asio::ssl::context m_sslContext;
	ssl_socket m_socket;
};

} // namespace SMTP