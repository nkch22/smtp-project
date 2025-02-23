/**
 * @file ISocket.h
 * @author Daniil
 * @brief Interface for a socket used in SMTP communication.
 * @version 0.1
 * @date 2025-02-23
 *
 * This interface defines the fundamental operations for a socket, including
 * synchronous and asynchronous sending and receiving of messages.
 *
 * @copyright Copyright (c) 2025
 */

#pragma once
#include <functional>
#include <string>

#include "SMTPResponse.h"

namespace SMTP
{

/**
 * @brief Delegate for handling asynchronous receive operations.
 */
using ReceiveDelegate = std::function<void(const SMTPResponse&)>;

/**
 * @brief Interface for socket communication.
 *
 * Defines methods for connecting, disconnecting, sending, and receiving messages
 * synchronously and asynchronously.
 *
 * @note If using async operations, consider inheriting from `std::enable_shared_from_this`.
 */
class ISocket
{
public:
	virtual ~ISocket() = default;

	/**
	 * @brief Connects to a specified server and port.
	 * @param server The address of the server.
	 * @param port The port number.
	 */
	virtual void Connect(const std::string& server, uint16_t port) = 0;

	/**
	 * @brief Disconnects from the server.
	 */
	virtual void Disconnect() = 0;

	/**
	 * @brief Sends a message synchronously.
	 * @param message The message to send.
	 */
	virtual void Send(const std::string& message) = 0;

	/**
	 * @brief Sends a message asynchronously.
	 * @param message The message to send.
	 */
	void AsyncSend(const std::string& message) { AsyncSendInternal(std::make_shared<std::string>(message)); }

	/**
	 * @brief Sends a message asynchronously using move semantics.
	 * @param message The message to send.
	 */
	void AsyncSend(std::string&& message) { AsyncSendInternal(std::make_shared<std::string>(std::move(message))); }

	/**
	 * @brief Receives a message synchronously.
	 * @return SMTPResponse The response received.
	 */
	virtual SMTPResponse Receive() = 0;

	/**
	 * @brief Receives a message asynchronously.
	 * @param receiveDelegate The callback function that handles the received response.
	 */
	virtual void AsyncReceive(ReceiveDelegate receiveDelegate) = 0;

	/**
	 * @brief Checks if the socket is currently connected.
	 * @return true if connected, false otherwise.
	 */
	bool IsConnected() { return m_isConnected; }

protected:
	/**
	 * @brief Internal function to send a message asynchronously.
	 * @param buffer The shared string buffer containing the message.
	 */
	virtual void AsyncSendInternal(std::shared_ptr<std::string> buffer) = 0;

protected:
	bool m_isConnected = false; ///< Connection status flag.
};

} // namespace SMTP
