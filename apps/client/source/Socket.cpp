#include "Socket.h"

using namespace SMTP;

Socket::Socket() : m_socket(m_context) {}

void Socket::Connect(const std::string& server, uint16_t port)
{
	m_isConnected = false;

	asio::ip::tcp::resolver resolver(m_context);
	asio::ip::tcp::resolver::query query(server, std::to_string(port));
	asio::ip::tcp::resolver::iterator endpoint = resolver.resolve(query);
	asio::ip::tcp::resolver::iterator end;

	asio::error_code error = asio::error::host_not_found;

	while (error && endpoint != end)
	{
		m_socket.close();
		m_socket.connect(*endpoint++, error);
	}

	if (error) throw asio::system_error(error);

	m_isConnected = true;
}

void Socket::Send(const std::string& message)
{
	asio::error_code error;
	m_socket.write_some(asio::buffer(message), error);
	if (error)
	{
		throw asio::system_error(error);
		m_isConnected = false;
	}
}

SMTPResponse Socket::Receive()
{
	char buffer[1024] = {};
	std::size_t bytesReceived = 0;

	try
	{
		bytesReceived = m_socket.receive(asio::buffer(buffer));
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(std::string("Something went wrong: ") + e.what());
		m_isConnected = false;
	}

	if (bytesReceived == 0) throw asio::system_error(asio::error::host_unreachable);

	SMTPResponse response;
	response.message = std::string(buffer, bytesReceived);
	response.code = static_cast<ResultCode>(std::stoi(response.message));

	return response;
}

void SMTP::Socket::AsyncReceive(ReceiveDelegate receiveDelegate)
{
	auto buffer = std::make_shared<std::array<char, 1024>>();

	m_socket.async_receive(asio::buffer(*buffer),
						   [buffer, receiveDelegate](const std::error_code& ec, std::size_t bytesReceived)
						   {
							   if (ec) throw asio::system_error(ec);

							   SMTPResponse response;
							   response.message = std::string(buffer->data(), bytesReceived);

							   response.code = static_cast<ResultCode>(std::stoi(response.message));
							   receiveDelegate(response);
						   });
}

// bool SMTP::Socket::IsConnected()
// {
// 	std::string message = "Ping";
// 	asio::error_code error;
// 	m_socket.send(asio::buffer(message), 0, error);
// }

void SMTP::Socket::AsyncSendInternal(std::shared_ptr<std::string> buffer)
{
	m_socket.async_write_some(asio::buffer(*buffer),
							  [buffer](const asio::error_code& error, std::size_t)
							  {
								  if (error) throw asio::system_error(error);
							  });
}

void SMTP::Socket::Disconnect()
{
	m_isConnected = false;

	try
	{
		m_socket.shutdown(asio::ip::tcp::socket::shutdown_both);
		m_socket.close();
	}
	catch (const asio::system_error& e)
	{
		throw std::runtime_error("Error while disconnecting SMTP socket: " + e.code().message());
	}
}
