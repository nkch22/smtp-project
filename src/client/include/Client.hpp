/**
 * @file Client.h
 * @author Daniil
 * @brief Implementation of SMTP client with support for regular and SSL connections.
 * @version 0.1
 * @date 2025-02-23
 *
 * This file defines the `Client` class, which is used as a base class for
 * sending email messages over SMTP. It provides methods for connecting to
 * an SMTP server, sending email, and quitting the session. Two derived classes
 *
 * @copyright Copyright (c) 2025
 */

#pragma once

#include <asio.hpp>
#include <asio/ssl.hpp>
#include <iostream>
#include <memory>

#include "Authenticator.hpp"
#include "ISocket.h"
#include "Mail.hpp"

namespace SMTP
{

/**
 * @brief Base class for SMTP client implementation.
 *
 * The `Client` class provides the core functionality for connecting
 * to an SMTP server, sending mail, and quitting the session. It also allows
 * for setting an authenticator, which can be used for authentication
 * during the connection process.
 */
class Client
{
protected:
	Client() = default;
	Client(std::unique_ptr<ISocket> socket, std::unique_ptr<IAuthenticator> authenticator) :
		m_socket(std::move(socket)), m_authenticator(std::move(authenticator))
	{
	}
	~Client();

public:
	Client(const Client&) = delete;
	Client& operator=(const Client&) = delete;

	/**
	 * @brief Initializes the SMTP client.
	 *
	 * This function sets up necessary resources for the client.
	 *
	 * @return True if initialization succeeds, false otherwise.
	 */
	static bool Init();

	/**
	 * @brief Shuts down the SMTP client.
	 *
	 * This function releases resources allocated by the client.
	 */
	static void Shutdown();

	/**
	 * @brief Connects to the SMTP server.
	 *
	 * This method establishes a connection to the SMTP server at the specified address
	 * and port.
	 *
	 * @param server The address of the SMTP server.
	 * @param port The port of the SMTP server.
	 */
	void Connect(const std::string& server, uint16_t port);

	/**
	 * @brief Sends an email message.
	 *
	 * This method sends the provided `Mail` object to the connected SMTP server.
	 *
	 * @param mail The email message to send.
	 */
	void SendMail(const Mail& mail);

	/**
	 * @brief Terminates the SMTP session.
	 *
	 * This method gracefully quits the SMTP session with the server.
	 */
	void Quit();

	/**
	 * @brief Get the singleton instance of the Client.
	 *
	 * @return Pointer to the Client instance.
	 */
	static Client* get_instance();

	/**
	 * @brief Get the username of the client.
	 *
	 * @return The username as a string.
	 */
	std::string get_username() const;

	/**
	 * @brief Get the password of the client.
	 *
	 * @return The password as a string.
	 */
	std::string get_password() const;

	/**
	 * @brief Sets the authenticator for authentication.
	 *
	 * This method allows setting a custom authenticator for handling the SMTP
	 * authentication process.
	 *
	 * @param authenticator The authenticator object to use.
	 */
	void set_authenticator(std::unique_ptr<IAuthenticator> authenticator);

	/**
	 * @brief Sets the socket.
	 *
	 * This method allows setting a custom socket for handling the SMTP
	 * connection process.
	 *
	 * @param socket The socket object to use.
	 */
	void set_socket(std::unique_ptr<ISocket> socket);

	/**
	 * @brief Set the username for authentication.
	 *
	 * @param username The username as a string.
	 */
	void set_username(const std::string& username);

	/**
	 * @brief Set the password for authentication.
	 *
	 * @param password The password as a string.
	 */
	void set_password(const std::string& password);

protected:
	/** @brief The socket used for connecting to the SMTP server. */
	std::unique_ptr<ISocket> m_socket;

	/** @brief The authenticator used for authentication. */
	std::unique_ptr<IAuthenticator> m_authenticator;

	/** @brief Username used for authentication. */
	std::string m_username;

	/** @brief User password used for authentication. */
	std::string m_password;

private:
	/** @brief Singleton instance of the Client. */
	static Client* s_Instance;

	/** @brief Mutex for thread safety. */
	static std::mutex s_Mutex;
};

} // namespace SMTP
