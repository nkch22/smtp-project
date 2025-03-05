#include "SSLSocket.h"

#include "Command.hpp"
using namespace SMTP;

SSLSocket::SSLSocket() : m_sslContext(asio::ssl::context::sslv23_client), m_socket(m_context, m_sslContext) {}

void SSLSocket::Connect(const std::string& server, uint16_t port)
{
	m_isConnected = false;

	asio::ip::tcp::resolver resolver(m_context);
	asio::ip::tcp::resolver::query query(server, std::to_string(port));
	asio::ip::tcp::resolver::iterator endpoint = resolver.resolve(query);
	asio::ip::tcp::resolver::iterator end;

	asio::error_code error = asio::error::host_not_found;
	while (error && endpoint != end)
	{
		m_socket.lowest_layer().close();
		asio::connect(m_socket.lowest_layer(), endpoint++, error);
	}

	if (error) throw asio::system_error(error);

	HandShake();

	m_isConnected = true;
}

void SSLSocket::Send(const std::string& message)
{
	asio::error_code error;
	asio::write(m_socket, asio::buffer(message), error);
	if (error) throw asio::system_error(error);
}

void SMTP::SSLSocket::AsyncSendInternal(std::shared_ptr<std::string> buffer)
{
	m_socket.async_write_some(asio::buffer(*buffer),
							  [buffer](const asio::error_code& error, std::size_t)
							  {
								  if (error) throw asio::system_error(error);
							  });
}

SMTPResponse SSLSocket::Receive()
{
	char buffer[1024] = {};
	std::size_t bytesReceived = 0;

	try
	{
		bytesReceived = m_socket.read_some(asio::buffer(buffer));
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(std::string("Something went wrong: ") + e.what());
		m_isConnected = false;
	}

	if (bytesReceived == 0)
	{
		throw asio::system_error(asio::error::host_unreachable);
		m_isConnected = false;
	}

	SMTPResponse response;
	response.message = std::string(buffer, bytesReceived);
	response.code = static_cast<ResultCode>(std::stoi(response.message));

	return response;
}

void SMTP::SSLSocket::AsyncReceive(ReceiveDelegate receiveDelegate)
{
	auto buffer = std::make_shared<std::array<char, 1024>>();
	m_socket.async_read_some(asio::buffer(*buffer),
							 [buffer, receiveDelegate](const std::error_code& ec, std::size_t bytesReceived)
							 {
								 if (ec) throw asio::system_error(ec);

								 SMTPResponse response;
								 response.message = std::string(buffer->data(), bytesReceived);

								 response.code = static_cast<ResultCode>(std::stoi(response.message));
								 receiveDelegate(response);
							 });
}

void SMTP::SSLSocket::Disconnect()
{
	m_isConnected = false;

	asio::error_code error;
	m_socket.shutdown(error);

	if (error) throw asio::system_error(error);

	try
	{
		m_socket.lowest_layer().close();
	}
	catch (const asio::system_error& e)
	{
		throw std::runtime_error("Error while disconnecting SMTP socket: " + e.code().message());
	}
}

void SSLSocket::HandShake()
{
	std::string serverName = m_socket.lowest_layer().remote_endpoint().address().to_string();
	asio::error_code error;

	AssertCode(Receive(), READY);
	Send(Command::EHLO(serverName));
	AssertCode(Receive(), OKAY);

	Send(Command::TLSS());

	auto response = Receive();

	switch (response.code)
	{
	case OKAY:
		m_socket.handshake(ssl_socket::client, error);
		if (error)
		{
			m_isConnected = false;
			throw asio::system_error(error);
		}
		break;
	case SYNTAX_ERROR:
		throw std::runtime_error("SMTP Error: Server responded with SYNTAX_ERROR");
	case TLS_NOT_AVAILABLE:
		throw std::runtime_error("SMTP Error: Server TLS not available due to temporary reason");
	default:
		throw std::runtime_error("SMTP Error: Unexpected response code: " + std::to_string(response.code));
	}
}
