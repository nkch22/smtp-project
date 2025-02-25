/**
 * @file Client.h
 * @author Daniil
 * @brief Implementation of SMTP client with support for regular and SSL connections.
 * @version 0.1
 * @date 2025-02-23
 *
 * This file defines the `ClientImpl` class, which is used as a base class for
 * sending email messages over SMTP. It provides methods for connecting to
 * an SMTP server, sending email, and quitting the session. Two derived classes
 * (`Client` and `SSLClient`) provide support for regular and SSL connections.
 *
 * @copyright Copyright (c) 2025
 */

#pragma once

#include <asio.hpp>
#include <asio/ssl.hpp>
#include <iostream>
#include <memory>

#include "Authenticator.hpp"
#include "Mail.hpp"
#include "SSLSocket.h"
#include "Socket.h"

namespace SMTP
{

/**
 * @brief Base class for SMTP client implementation.
 *
 * The `ClientImpl` class provides the core functionality for connecting
 * to an SMTP server, sending mail, and quitting the session. It also allows
 * for setting an authenticator, which can be used for authentication
 * during the connection process.
 *
 * The derived classes `Client` and `SSLClient` specialize in regular
 * and SSL connections respectively.
 */
class ClientImpl
{
public:
	ClientImpl() = default;
	virtual ~ClientImpl();

	/**
	 * @brief Initializes the SMTP client with an optional authenticator.
	 *
	 * This method is used to initialize the client with an optional authenticator
	 * for handling authentication during the SMTP connection.
	 *
	 * @param authenticator An optional authenticator object for handling authentication.
	 * @return `true` if initialization is successful, `false` otherwise.
	 */
	virtual bool Init(std::unique_ptr<IAuthenticator> authenticator = nullptr) = 0;

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
	 * @brief Sets the authenticator for authentication.
	 *
	 * This method allows setting a custom authenticator for handling the SMTP
	 * authentication process.
	 *
	 * @param authenticator The authenticator object to use.
	 */
	void set_authenticator(std::unique_ptr<IAuthenticator> authenticator);

protected:
	/** The socket used for connecting to the SMTP server. */
	std::unique_ptr<ISocket> m_socket;

	/** The authenticator used for authentication. */
	std::unique_ptr<IAuthenticator> m_authenticator;
};

/**
 * @brief Client class for regular SMTP connections.
 *
 * The `Client` class implements the `Init` method from `ClientImpl` and provides
 * functionality for sending emails over a regular SMTP connection.
 */
class Client : public ClientImpl
{
public:
	Client() = default;
	~Client() = default;

	/**
	 * @brief Initializes the client with an optional authenticator.
	 *
	 * This method overrides the `Init` method from `ClientImpl` to implement the
	 * initialization logic specific to regular SMTP connections.
	 *
	 * @param authenticator An optional authenticator object for handling authentication.
	 * @return `true` if initialization is successful, `false` otherwise.
	 */
	bool Init(std::unique_ptr<IAuthenticator> authenticator = nullptr) override;
};

/**
 * @brief Client class for SMTP connections over SSL.
 *
 * The `SSLClient` class implements the `Init` method from `ClientImpl` and provides
 * functionality for sending emails over an SMTP connection secured with SSL/TLS.
 */
class SSLClient : public ClientImpl
{
public:
	SSLClient() = default;
	~SSLClient() = default;

	/**
	 * @brief Initializes the client with an optional authenticator.
	 *
	 * This method overrides the `Init` method from `ClientImpl` to implement the
	 * initialization logic specific to SSL-secured SMTP connections.
	 *
	 * @param authenticator An optional authenticator object for handling authentication.
	 * @return `true` if initialization is successful, `false` otherwise.
	 */
	bool Init(std::unique_ptr<IAuthenticator> authenticator = nullptr) override;
};

} // namespace SMTP
