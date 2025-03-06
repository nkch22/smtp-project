/**
 * @file SMTPResponse.h
 * @author Daniil
 * @brief Defines SMTP result codes and response structure.
 * @version 0.1
 * @date 2025-02-23
 *
 * This file defines the various result codes for SMTP communication,
 * as well as the `SMTPResponse` struct that holds the response message
 * and code. Additionally, it provides a utility function `AssertCode`
 * to verify the correctness of an SMTP response.
 *
 * @copyright Copyright (c) 2025
 */

#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

namespace SMTP
{

/**
 * @brief Enum for SMTP result codes.
 *
 * These result codes are used in SMTP communication to indicate various
 * states or responses from the server.
 */
enum ResultCode
{
	NONE = 0,				 ///< No result code.
	READY = 220,			 ///< SMTP server is ready.
	GOODBYE = 221,			 ///< SMTP server is closing the connection.
	AUTH_SUCCESSFUL = 235,	 ///< Authentication was successful.
	OKAY = 250,				 ///< Command executed successfully.
	ACCEPTED = 334,			 ///< Server is awaiting further authentication information.
	DATA = 354,				 ///< Server is ready to receive data.
	TLS_NOT_AVAILABLE = 454, ///< TLS is not available.
	SYNTAX_ERROR = 501,		 ///< Syntax error in the command.
};

/**
 * @brief Struct to hold SMTP responses.
 *
 * This struct contains the message and the result code returned by the
 * SMTP server, allowing easy access to both pieces of information.
 */
struct SMTPResponse
{
	/**
	 * @brief Default constructor initializes with an empty message and NONE code.
	 */
	SMTPResponse() : message(""), code(ResultCode::NONE) {};

	/**
	 * @brief Constructs a new SMTPResponse object with a message and code.
	 * @param message The message from the SMTP server.
	 * @param code The result code from the SMTP server.
	 */
	SMTPResponse(std::string message, ResultCode code) : message(message), code(code) {}

	/**
	 * @brief Move constructor.
	 * @param other The SMTPResponse to move from.
	 */
	SMTPResponse(SMTPResponse&& other) noexcept : message(std::move(other.message)), code(other.code) {}

	/**
	 * @brief Copy constructor.
	 * @param other The SMTPResponse to copy from.
	 */
	SMTPResponse(const SMTPResponse& other) : message(other.message), code(other.code) {}

	/**
	 * @brief Copy assignment operator.
	 * @param other The SMTPResponse to copy from.
	 * @return A reference to the updated object.
	 */
	SMTPResponse& operator=(const SMTPResponse& other)
	{
		if (this != &other)
		{
			message = other.message;
			code = other.code;
		}
		return *this;
	}

	/**
	 * @brief Move assignment operator.
	 * @param other The SMTPResponse to move from.
	 * @return A reference to the updated object.
	 */
	SMTPResponse& operator=(const SMTPResponse&& other)
	{
		if (this != &other)
		{
			message = std::move(other.message);
			code = other.code;
		}
		return *this;
	}

	std::string message; ///< The message from the SMTP server.
	ResultCode code;	 ///< The result code from the SMTP server.
};

/**
 * @brief Asserts that the SMTP response code matches the expected code.
 *
 * This function checks if the response code matches the expected code
 * and throws a runtime error if it does not.
 *
 * @param response The SMTP response to check.
 * @param expected The expected result code.
 * @throws std::runtime_error If the response code does not match the expected code.
 */
inline void AssertCode(const SMTPResponse& response, ResultCode expected)
{
	std::cout << "Response: " << response.message << std::endl;
	if (response.code != expected)
	{
		throw std::runtime_error("Unexpected SMTP response code: " + std::to_string(response.code)
								 + " expected: " + std::to_string(expected));
	}
}

} // namespace SMTP
