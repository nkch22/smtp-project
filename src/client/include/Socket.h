/**
 * @file Socket.h
 * @author Daniil
 * @brief Implementation of the ISocket interface using ASIO.
 * @version 0.1
 * @date 2025-02-23
 *
 * This class provides an implementation of the ISocket interface using
 * ASIO for synchronous and asynchronous socket communication.
 *
 * @copyright Copyright (c) 2025
 */

#pragma once
#include <asio.hpp>

#include "ISocket.h"

namespace SMTP
{

/**
 * @brief Implementation of the ISocket interface using ASIO.
 *
 * This class provides methods for connecting, disconnecting, sending,
 * and receiving data using TCP sockets. It utilizes the ASIO library
 * for asynchronous operations.
 */
class Socket : public ISocket
{
public:
	Socket();
	~Socket() = default;

	/**
	 * @brief Connects to the specified server and port.
	 * @param server The address of the server.
	 * @param port The port number.
	 */
	void Connect(const std::string& server, uint16_t port) override;

	/**
	 * @brief Disconnects from the server.
	 */
	void Disconnect() override;

	/**
	 * @brief Sends a message synchronously.
	 * @param message The message to send.
	 */
	void Send(const std::string& message) override;

	/**
	 * @brief Receives a message synchronously.
	 * @return SMTPResponse The response received.
	 */
	SMTPResponse Receive() override;

	/**
	 * @brief Receives a message asynchronously.
	 * @param receiveDelegate The callback function that handles the received response.
	 */
	void AsyncReceive(ReceiveDelegate receiveDelegate) override;

private:
	/**
	 * @brief Internal function to send a message asynchronously.
	 * @param buffer The shared string buffer containing the message.
	 */
	void AsyncSendInternal(std::shared_ptr<std::string> buffer) override;

private:
	asio::io_context m_context;		///< ASIO IO context for managing asynchronous operations.
	asio::ip::tcp::socket m_socket; ///< The TCP socket used for communication.
};

} // namespace SMTP
