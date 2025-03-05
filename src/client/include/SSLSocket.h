/**
 * @file SSLSocket.h
 * @author Daniil
 * @brief Implementation of the ISocket interface with SSL support.
 * @version 0.1
 * @date 2025-02-23
 *
 * This class provides an implementation of the ISocket interface, extending
 * the functionality to support secure socket communication over SSL/TLS using ASIO.
 *
 * @copyright Copyright (c) 2025
 */

#pragma once
#include <asio.hpp>
#include <asio/ssl.hpp>

#include "ISocket.h"

namespace SMTP
{

/**
 * @brief Implementation of the ISocket interface with SSL/TLS support using ASIO.
 *
 * This class provides secure socket communication using SSL/TLS for encrypted
 * data transmission. It utilizes the ASIO library for managing the network
 * operations and the SSL layer for encryption.
 */
class SSLSocket : public ISocket
{
public:
	/**
	 * @brief Type definition for SSL socket stream.
	 */
	typedef asio::ssl::stream<asio::ip::tcp::socket> ssl_socket;

public:
	/**
	 * @brief Constructs a new SSLSocket object.
	 */
	SSLSocket();

	/**
	 * @brief Destroys the SSLSocket object.
	 */
	~SSLSocket() = default;

	/**
	 * @brief Connects to the specified server and port over a secure SSL connection.
	 * @param server The address of the server.
	 * @param port The port number.
	 */
	void Connect(const std::string& server, uint16_t port) override;

	/**
	 * @brief Disconnects from the server.
	 */
	void Disconnect() override;

	/**
	 * @brief Sends a message synchronously over the SSL connection.
	 * @param message The message to send.
	 */
	void Send(const std::string& message) override;

	/**
	 * @brief Receives a message synchronously over the SSL connection.
	 * @return SMTPResponse The response received.
	 */
	SMTPResponse Receive() override;

	/**
	 * @brief Receives a message asynchronously over the SSL connection.
	 * @param receiveDelegate The callback function that handles the received response.
	 */
	void AsyncReceive(ReceiveDelegate receiveDelegate) override;

private:
	/**
	 * @brief Internal function to send a message asynchronously over the SSL connection.
	 * @param buffer The shared string buffer containing the message.
	 */
	void AsyncSendInternal(std::shared_ptr<std::string> buffer) override;

	/**
	 * @brief Performs the SSL handshake to establish a secure connection.
	 */
	void HandShake();

private:
	asio::io_context m_context;		 ///< ASIO IO context for managing asynchronous operations.
	asio::ssl::context m_sslContext; ///< SSL context used to configure SSL/TLS operations.
	ssl_socket m_socket;			 ///< The SSL socket used for secure communication.
};

} // namespace SMTP
