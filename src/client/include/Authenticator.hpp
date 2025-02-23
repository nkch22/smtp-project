/**
 * @file Authenticator.h
 * @author Daniil
 * @brief Interface and implementations for SMTP authentication methods.
 * @version 0.1
 * @date 2025-02-23
 *
 * This file defines the `IAuthenticator` interface and two derived classes (`AuthPlain`, `AuthLogin`)
 * for performing different types of SMTP authentication. These authentication methods are used
 * to authenticate the user against the SMTP server.
 *
 * @copyright Copyright (c) 2025
 */

#pragma once
#include <string>

#include "ISocket.h"

namespace SMTP
{

/**
 * @brief Interface for SMTP authentication methods.
 *
 * The `IAuthenticator` interface provides a common method for performing
 * SMTP authentication. Derived classes must implement the `Authenticate`
 * method to provide specific authentication logic.
 */
class IAuthenticator
{
public:
	IAuthenticator() = default;
	virtual ~IAuthenticator() = default;

	/**
	 * @brief Performs authentication on the given socket with the specified username and password.
	 *
	 * This method is intended to be implemented by derived classes to perform
	 * the actual authentication logic for SMTP servers.
	 *
	 * @param socket The socket connected to the SMTP server.
	 * @param username The username for authentication.
	 * @param password The password for authentication.
	 * @return `true` if authentication is successful, `false` otherwise.
	 */
	virtual bool Authenticate(ISocket& socket, const std::string& username, const std::string& password) = 0;
};

/**
 * @brief Plain text SMTP authentication.
 *
 * The `AuthPlain` class implements the `IAuthenticator` interface to provide
 * plain text authentication for SMTP servers. It sends the username and
 * password in a plain text format.
 */
class AuthPlain : public IAuthenticator
{
public:
	AuthPlain() = default;
	virtual ~AuthPlain() = default;

	/**
	 * @brief Performs plain text authentication with the SMTP server.
	 *
	 * This method sends the username and password in plain text format
	 * to authenticate the user with the SMTP server.
	 *
	 * @param socket The socket connected to the SMTP server.
	 * @param username The username for authentication.
	 * @param password The password for authentication.
	 * @return `true` if authentication is successful, `false` otherwise.
	 */
	bool Authenticate(ISocket& socket, const std::string& username, const std::string& password) override;
};

/**
 * @brief Login-based SMTP authentication.
 *
 * The `AuthLogin` class implements the `IAuthenticator` interface to provide
 * login-based authentication for SMTP servers. This method sends the username
 * and password using the SMTP LOGIN command.
 */
class AuthLogin : public IAuthenticator
{
public:
	AuthLogin() = default;
	virtual ~AuthLogin() = default;

	/**
	 * @brief Performs login authentication with the SMTP server.
	 *
	 * This method sends the username and password using the SMTP LOGIN command
	 * to authenticate the user with the SMTP server.
	 *
	 * @param socket The socket connected to the SMTP server.
	 * @param username The username for authentication.
	 * @param password The password for authentication.
	 * @return `true` if authentication is successful, `false` otherwise.
	 */
	bool Authenticate(ISocket& socket, const std::string& username, const std::string& password) override;
};

} // namespace SMTP
